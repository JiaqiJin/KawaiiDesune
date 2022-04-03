#pragma once

namespace RHI
{
	class RenderDevice;
	class DX12DescriptorCache;

	class DX12CommandContext
	{
	public:
		DX12CommandContext(RenderDevice* device);
		~DX12CommandContext();

		void CreateCommandContext();
		void DestroyCommandContext();
		void ResetCommandAllocator();
		void ResetCommandList();
		void ExecuteCommandLists();
		void FlushCommandQueue();

		// Getters
		ID3D12CommandQueue* GetCommandQueue() { return CommandQueue.Get(); }
		ID3D12GraphicsCommandList* GetCommandList() { return CommandList.Get(); }

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
		// Descriptor Cache
		std::unique_ptr<DX12DescriptorCache> m_DescriptorCache = nullptr;
	};
}