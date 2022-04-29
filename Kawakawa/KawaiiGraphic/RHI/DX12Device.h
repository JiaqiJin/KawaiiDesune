#pragma once

#include "DX12CommandContext.h"
#include "DX12HeapSlotAllocator.h"
#include "DX12MemoryAllocator.h"

namespace RHI
{
	class DX12GraphicRHI;

	class RenderDevice
	{
	public:
		RenderDevice(DX12GraphicRHI* D3D12RHI);
		~RenderDevice();

		// Gettes
		ID3D12Device* GetD3DDevice() { return m_Device.Get(); }
		DX12CommandContext* GetCommandContext() { return m_CommandContext.get(); }
		ID3D12CommandQueue* GetCommandQueue() { return m_CommandContext->GetCommandQueue(); }
		ID3D12GraphicsCommandList* GetCommandList() { return m_CommandContext->GetCommandList(); }
		DX12HeapSlotAllocator* GetHeapSlotAllocator(D3D12_DESCRIPTOR_HEAP_TYPE HeapType);
	private:
		// Initialize the Device
		void Initialize();

	private:
		DX12GraphicRHI* m_D3D12RHI = nullptr;

		Microsoft::WRL::ComPtr<ID3D12Device> m_Device = nullptr;
		// Command Context
		std::unique_ptr<DX12CommandContext> m_CommandContext = nullptr;
		
		// Heap Slot Allocators
		std::unique_ptr<DX12HeapSlotAllocator> m_RTVHeapSlotAllocator = nullptr;
		std::unique_ptr<DX12HeapSlotAllocator> m_DSVHeapSlotAllocator = nullptr;
		std::unique_ptr<DX12HeapSlotAllocator> m_SRVHeapSlotAllocator = nullptr;

		// Mmeory Allocators
		std::unique_ptr<DX12UploadBufferAllocator> m_UploadBufferAllocator = nullptr;
		std::unique_ptr<DX12DefaultBufferAllocator> m_DefaultBufferAllocator = nullptr;
		std::unique_ptr<DX12TextureResourceAllocator> m_TextureResourceAllocator = nullptr;
	};
}