#include "pch.h"
#include "DX12Device.h"

using Microsoft::WRL::ComPtr;

namespace RHI
{
	RenderDevice::RenderDevice()
	{
		Initialize();
	}

	RenderDevice::~RenderDevice()
	{

	}

	void RenderDevice::Initialize()
	{
		// D3D12 debug
		UINT DxgiFactoryFlags = 0;

#if (defined(DEBUG) || defined(_DEBUG))  
		{
			ComPtr<ID3D12Debug> DebugController;
			ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(DebugController.GetAddressOf())));
			DebugController->EnableDebugLayer();
		}

		ComPtr<IDXGIInfoQueue> DxgiInfoQueue;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(DxgiInfoQueue.GetAddressOf()))))
		{
			DxgiFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;

			DxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
			DxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
		}

#endif
		// Create DxgiFactory
		ThrowIfFailed(CreateDXGIFactory2(DxgiFactoryFlags, IID_PPV_ARGS(DxgiFactory.GetAddressOf())));

		// Try to create hardware device.
		HRESULT HardwareResult = D3D12CreateDevice(nullptr/*default adapter*/, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device));

		// Fallback to WARP device.
		if (FAILED(HardwareResult))
		{
			ComPtr<IDXGIAdapter> WarpAdapter;
			ThrowIfFailed(DxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&WarpAdapter)));

			ThrowIfFailed(D3D12CreateDevice(WarpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)));
		}

		//Create CommandContext
		m_CommandContext = std::make_unique<DX12CommandContext>(this);

		//Create heapSlot allocator
		m_RTVHeapSlotAllocator = std::make_unique<DX12HeapSlotAllocator>(m_Device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 200);
		m_DSVHeapSlotAllocator = std::make_unique<DX12HeapSlotAllocator>(m_Device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 200);
		m_SRVHeapSlotAllocator = std::make_unique<DX12HeapSlotAllocator>(m_Device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 200);
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