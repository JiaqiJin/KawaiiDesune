#include "pch.h"
#include "CommandListManager.h"

namespace RHI
{
	CommandListManager::CommandListManager()
		: m_Device(nullptr),
		m_GraphicsQueue(D3D12_COMMAND_LIST_TYPE_DIRECT),
		m_ComputeQueue(D3D12_COMMAND_LIST_TYPE_COMPUTE),
		m_CopyQueue(D3D12_COMMAND_LIST_TYPE_COPY)
	{

	}

	CommandListManager::~CommandListManager()
	{
		Shutdown();
	}

	void CommandListManager::Create(ID3D12Device* pDevice)
	{
		assert(pDevice != nullptr);
		
		m_Device = pDevice;

		m_GraphicsQueue.Create(pDevice);
		m_ComputeQueue.Create(pDevice);
		m_CopyQueue.Create(pDevice);
	}

	void CommandListManager::Shutdown()
	{
		m_GraphicsQueue.ShutDown();
		m_ComputeQueue.ShutDown();
		m_CopyQueue.ShutDown();
	}

	void CommandListManager::CreateNewCommandList(
		D3D12_COMMAND_LIST_TYPE type,
		ID3D12GraphicsCommandList** list,
		ID3D12CommandAllocator** allocator)
	{
		ASSERT(type != D3D12_COMMAND_LIST_TYPE_BUNDLE, "Bundles are not yet supported");

		switch (type)
		{
		case D3D12_COMMAND_LIST_TYPE_DIRECT:
			*allocator = m_GraphicsQueue.RequestAllocator();
			break;
		case D3D12_COMMAND_LIST_TYPE_BUNDLE:
			break;
		case D3D12_COMMAND_LIST_TYPE_COMPUTE:
			*allocator = m_ComputeQueue.RequestAllocator();
			break;
		case D3D12_COMMAND_LIST_TYPE_COPY:
			*allocator = m_CopyQueue.RequestAllocator();
			break;
		}

		ASSERT_SUCCEEDED(m_Device->CreateCommandList(1, type, *allocator, nullptr, IID_PPV_ARGS(list)));
		(*list)->SetName(L"CommandList");
	}

	void CommandListManager::WaitForFence(uint64_t fenceValue)
	{
		GetQueue((D3D12_COMMAND_LIST_TYPE)(fenceValue >> 56)).WaitForFence(fenceValue);
	}
}