#include "pch.h"
#include "DX12Device.h"
#include "DX12GraphicRHI.h"

using Microsoft::WRL::ComPtr;

namespace RHI
{
	RenderDevice::RenderDevice(DX12GraphicRHI* DX12RHI)
		: m_D3D12RHI(DX12RHI)
	{
		Initialize();
	}

	RenderDevice::~RenderDevice()
	{

	}

	void RenderDevice::Initialize()
	{
		// Try to create hardware device.
		HRESULT HardwareResult = D3D12CreateDevice(nullptr/*default adapter*/, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device));

		// Fallback to WARP device.
		if (FAILED(HardwareResult))
		{
			ComPtr<IDXGIAdapter> WarpAdapter;
			ThrowIfFailed(m_D3D12RHI->GetDxgiFactory()->EnumWarpAdapter(IID_PPV_ARGS(&WarpAdapter)));

			ThrowIfFailed(D3D12CreateDevice(WarpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)));
		}

		// Create CommandContext
		m_CommandContext = std::make_unique<DX12CommandContext>(this);

		// Create heapSlot allocator
		m_RTVHeapSlotAllocator = std::make_unique<DX12HeapSlotAllocator>(m_Device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 200);
		m_DSVHeapSlotAllocator = std::make_unique<DX12HeapSlotAllocator>(m_Device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 200);
		m_SRVHeapSlotAllocator = std::make_unique<DX12HeapSlotAllocator>(m_Device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 200);

		// Create Allocator
		m_UploadBufferAllocator = std::make_unique<DX12UploadBufferAllocator>(m_Device.Get());
		m_DefaultBufferAllocator = std::make_unique<DX12DefaultBufferAllocator>(m_Device.Get());
		m_TextureResourceAllocator = std::make_unique<DX12TextureResourceAllocator>(m_Device.Get());
	}

	DX12HeapSlotAllocator* RenderDevice::GetHeapSlotAllocator(D3D12_DESCRIPTOR_HEAP_TYPE HeapType)
	{
		switch (HeapType)
		{
		case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
			return m_SRVHeapSlotAllocator.get();
			break;
		case D3D12_DESCRIPTOR_HEAP_TYPE_RTV:
			return m_RTVHeapSlotAllocator.get();
			break;
		case D3D12_DESCRIPTOR_HEAP_TYPE_DSV:
			return m_DSVHeapSlotAllocator.get();
			break;
		default:
			return nullptr;
		}
	}
}