#include "pch.h"
#include "CommandQueue.h"
#include "CommandAllocatorPool.h"

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
		CloseHandle(m_FenceEventHandle);
	}
}