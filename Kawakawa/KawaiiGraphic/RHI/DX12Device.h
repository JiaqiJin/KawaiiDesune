#pragma once

#include "DX12CommandContext.h"
#include "DX12HeapSlotAllocator.h"

namespace RHI
{
	class RenderDevice
	{
	public:
		RenderDevice();
		~RenderDevice();

		ID3D12Device* GetD3DDevice() { return m_Device.Get(); }
		DX12CommandContext* GetCommandContext() { return m_CommandContext.get(); }
		ID3D12CommandQueue* GetCommandQueue() { return m_CommandContext->GetCommandQueue(); }
		ID3D12GraphicsCommandList* GetCommandList() { return m_CommandContext->GetCommandList(); }
		DX12HeapSlotAllocator* GetHeapSlotAllocator(D3D12_DESCRIPTOR_HEAP_TYPE HeapType);
	private:
		void Initialize();

	private:
		Microsoft::WRL::ComPtr<ID3D12Device> m_Device = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory4> DxgiFactory = nullptr;
		// Command Context
		std::unique_ptr<DX12CommandContext> m_CommandContext = nullptr;
		
		// Heap Slot Allocators
		std::unique_ptr<DX12HeapSlotAllocator> m_RTVHeapSlotAllocator = nullptr;
		std::unique_ptr<DX12HeapSlotAllocator> m_DSVHeapSlotAllocator = nullptr;
		std::unique_ptr<DX12HeapSlotAllocator> m_SRVHeapSlotAllocator = nullptr;
	};
}