#include "pch.h"
#include "CommandQueue.h"

namespace RHI
{
	CommandQueue::CommandQueue(D3D12_COMMAND_LIST_TYPE type)
		: m_Type(type),
		m_CommandQueue(nullptr),
		m_pFence(nullptr),
		m_NextFenceValue((uint64_t)type << 56 | 1),
		m_LastCompletedFenceValue((uint64_t)type << 56),
		m_AllocatorPool(type)
	{

	}

	CommandQueue::~CommandQueue()
	{
		ShutDown();
	}

	void CommandQueue::Create(ID3D12Device* pDevice)
	{
		ASSERT(pDevice != nullptr);
		ASSERT(!IsReady());
		ASSERT(m_AllocatorPool.Size() == 0);

		// Create Command Queue
		D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
		cmdQueueDesc.Type = m_Type;
		cmdQueueDesc.NodeMask = 1;
		pDevice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_CommandQueue));
		m_CommandQueue->SetName(L"CommandListManager::m_CommandQueue");

		// Create Fence
		ASSERT_SUCCEEDED(pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence)));
		m_pFence->SetName(L"CommandListManager::m_pFence");
		m_pFence->Signal((uint64_t)m_Type << 56);

		m_FenceEventHandle = CreateEvent(nullptr, false, false, nullptr);
		ASSERT(m_FenceEventHandle != nullptr);

		m_AllocatorPool.Create(pDevice);

		ASSERT(IsReady());
	}

	void CommandQueue::ShutDown()
	{
		if (m_CommandQueue == nullptr)
			return;

		m_AllocatorPool.Shutdown();

		CloseHandle(m_FenceEventHandle);

		m_pFence->Release();
		m_pFence = nullptr;

		m_CommandQueue->Release();
		m_CommandQueue = nullptr;
	}

	uint64_t CommandQueue::IncrementFence()
	{
		std::lock_guard<std::mutex> lockGuard(m_FenceMutex);

		m_CommandQueue->Signal(m_pFence, m_NextFenceValue);
		return m_NextFenceValue++;
	}

	bool CommandQueue::IsFenceComplete(uint64_t fenceValue)
	{
		if (fenceValue > m_LastCompletedFenceValue)
			m_LastCompletedFenceValue = std::max(m_LastCompletedFenceValue, m_pFence->GetCompletedValue());

		return fenceValue <= m_LastCompletedFenceValue;
	}

	void CommandQueue::WaitForFence(uint64_t fenceValue)
	{
		if (IsFenceComplete(fenceValue))
			return;

		{
			std::lock_guard<std::mutex> lockGuard(m_EventMutex);

			m_pFence->SetEventOnCompletion(fenceValue, m_FenceEventHandle);
			WaitForSingleObject(m_FenceEventHandle, INFINITE);
			m_LastCompletedFenceValue = fenceValue;
		}
	}

	uint64_t CommandQueue::ExecuteCommandList(ID3D12CommandList* list)
	{
		std::lock_guard<std::mutex> lockGuard(m_FenceMutex);

		ASSERT_SUCCEEDED(((ID3D12GraphicsCommandList*)list)->Close());

		// kickoff the command list
		m_CommandQueue->ExecuteCommandLists(1, &list);

		// signal the next fence value (with the GPU)
		m_CommandQueue->Signal(m_pFence, m_NextFenceValue);

		// and increment the fence value
		return m_NextFenceValue++;
	}

	ID3D12CommandAllocator* CommandQueue::RequestAllocator()
	{
		uint64_t fenceValue = m_pFence->GetCompletedValue();
		return m_AllocatorPool.RequestAllocator(fenceValue);
	}

	void CommandQueue::DiscardAllocator(uint64_t fenceValueForReset, ID3D12CommandAllocator* allocator)
	{
		m_AllocatorPool.DiscardAllocator(fenceValueForReset, allocator);
	}
}