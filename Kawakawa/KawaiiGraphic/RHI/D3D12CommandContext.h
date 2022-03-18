#pragma once

namespace RHI
{
	class RenderDevice;

	class D3D12CommandContext
	{
	public:
		D3D12CommandContext(RenderDevice* device);
		~D3D12CommandContext();

		void ResetCommandAllocator();
		void ResetCommandList();
		void ExecuteCommandLists();
		void FlushCommandQueue();
		// Getters
		ID3D12CommandQueue* GetCommandQueue() { return CommandQueue.Get(); }
		ID3D12GraphicsCommandList* GetCommandList() { return CommandList.Get(); }

	private:
		void CreateCommandContext();

		void DestroyCommandContext();

	private:
		RenderDevice* m_Device;
		// Command List
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList = nullptr;
		// Command Queue
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> CommandQueue = nullptr;
		// Command Allocator
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandListAlloc = nullptr;
		// Fence
		Microsoft::WRL::ComPtr<ID3D12Fence> Fence = nullptr;
		// Fence Value
		UINT64 CurrentFenceValue = 0;
	};
}