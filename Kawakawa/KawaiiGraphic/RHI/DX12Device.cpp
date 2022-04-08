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
	}
}