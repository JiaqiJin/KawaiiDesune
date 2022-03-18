#include "pch.h"
#include "D3D12CommandContext.h"
#include "D3D12Device.h"
#include "D3D12Utils.h"

namespace RHI
{
	D3D12CommandContext::D3D12CommandContext(RenderDevice* device)
		: m_Device(device)
	{
		CreateCommandContext();
	}

	D3D12CommandContext::~D3D12CommandContext()
	{
		DestroyCommandContext();
	}

	void D3D12CommandContext::CreateCommandContext()
	{
		//Create fence
		ThrowIfFailed(m_Device->GetD3DDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));

		//Create direct type commandQueue
		D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
		QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		ThrowIfFailed(m_Device->GetD3DDevice()->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));

		//Create direct type commandAllocator
		ThrowIfFailed(m_Device->GetD3DDevice()->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(CommandListAlloc.GetAddressOf())));

		//Create direct type commandList
		ThrowIfFailed(m_Device->GetD3DDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandListAlloc.Get(),
			nullptr, IID_PPV_ARGS(CommandList.GetAddressOf())));

		// Start off in a closed state. 
		// This is because the first time we refer to the command list we will Reset it,
		// and it needs to be closed before calling Reset.
		ThrowIfFailed(CommandList->Close());
	}

	void D3D12CommandContext::DestroyCommandContext()
	{

	}

	void D3D12CommandContext::ResetCommandAllocator()
	{
		ThrowIfFailed(CommandListAlloc->Reset());
	}

	void D3D12CommandContext::ResetCommandList()
	{
		ThrowIfFailed(CommandList->Reset(CommandListAlloc.Get(), nullptr));
	}

	void D3D12CommandContext::ExecuteCommandLists()
	{
		// Done recording commands.
		ThrowIfFailed(CommandList->Close());

		// Add the command list to the queue for execution.
		ID3D12CommandList* cmdsLists[] = { CommandList.Get() };
		CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	}

	void D3D12CommandContext::FlushCommandQueue()
	{
		// Advance the fence value to mark commands up to this fence point.
		CurrentFenceValue++;

		// Add an instruction to the command queue to set a new fence point.  
		// Because we are on the GPU timeline, the new fence point won't be set until the GPU finishes
		// processing all the commands prior to this Signal().
		ThrowIfFailed(CommandQueue->Signal(Fence.Get(), CurrentFenceValue));

		// Wait until the GPU has completed commands up to this fence point.
		if (Fence->GetCompletedValue() < CurrentFenceValue)
		{
			HANDLE eventHandle = CreateEvent(nullptr, false, false, nullptr);

			// Fire event when GPU hits current fence.  
			ThrowIfFailed(Fence->SetEventOnCompletion(CurrentFenceValue, eventHandle));

			// Wait until the GPU hits current fence event is fired.
			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}
	}
}