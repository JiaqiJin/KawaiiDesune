#include "D3Dpch.h"
#include "CommandListManager.h"

namespace Kawaii::Graphics::backend::DX12
{
	CommandListManager::CommandListManager(ID3D12Device* Device) :
		m_Device(Device),
		m_GraphicsQueue(D3D12_COMMAND_LIST_TYPE_DIRECT, Device),
		m_ComputeQueue(D3D12_COMMAND_LIST_TYPE_COMPUTE, Device),
		m_CopyQueue(D3D12_COMMAND_LIST_TYPE_COPY, Device)
	{

	}

	void CommandListManager::CreateNewCommandList(D3D12_COMMAND_LIST_TYPE Type, ID3D12GraphicsCommandList5** List,
		ID3D12CommandAllocator** Allocator)
	{
		assert(Type != D3D12_COMMAND_LIST_TYPE_BUNDLE && "Bundles are not yet supported");
		switch (Type)
		{
		case D3D12_COMMAND_LIST_TYPE_DIRECT: *Allocator = m_GraphicsQueue.RequestAllocator(); break;
		case D3D12_COMMAND_LIST_TYPE_BUNDLE: break;
		case D3D12_COMMAND_LIST_TYPE_COMPUTE: *Allocator = m_ComputeQueue.RequestAllocator(); break;
		case D3D12_COMMAND_LIST_TYPE_COPY: *Allocator = m_CopyQueue.RequestAllocator(); break;
		}

		ThrowIfFailed(m_Device->CreateCommandList(1, Type, *Allocator, nullptr, IID_PPV_ARGS(List)));
		(*List)->SetName(L"CommandList");
	}

	void CommandListManager::WaitForFence(uint64_t FenceValue)
	{
		switch (static_cast<D3D12_COMMAND_LIST_TYPE>(FenceValue >> 56))
		{
		case D3D12_COMMAND_LIST_TYPE_DIRECT:
			m_GraphicsQueue.WaitForFence(FenceValue);
			break;
		case D3D12_COMMAND_LIST_TYPE_COMPUTE:
			m_ComputeQueue.WaitForFence(FenceValue);
			break;
		case D3D12_COMMAND_LIST_TYPE_COPY:
			m_CopyQueue.WaitForFence(FenceValue);
			break;
		default:
			throw "[D3D CommandListManager] Unsupport Command List Type!";
			break;
		}
	}

}