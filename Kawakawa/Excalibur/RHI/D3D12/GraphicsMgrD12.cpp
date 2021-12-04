#include "GraphicsMgrD12.h"

namespace Excalibur
{
	int	GraphicsMgrD12::Initialize() noexcept
	{
		return 0;
	}

	int GraphicsMgrD12::InitializeWithWindow(HWND handler) noexcept
	{
#if defined(DEBUG) || defined(_DEBUG) 
		// Enable the D3D12 debug layer.
		{
			ComPtr<ID3D12Debug> debugController;
			ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
			debugController->EnableDebugLayer();
		}
#endif

		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory)));

		// Try to create hardware device.
		HRESULT hardwareResult = D3D12CreateDevice(
			nullptr,             // default adapter
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&m_Device));

		// Fallback to WARP device.
		if (FAILED(hardwareResult))
		{
			ComPtr<IDXGIAdapter> pWarpAdapter;
			ThrowIfFailed(m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));

			ThrowIfFailed(D3D12CreateDevice(
				pWarpAdapter.Get(),
				D3D_FEATURE_LEVEL_11_0,
				IID_PPV_ARGS(&m_Device)));
		}

		ThrowIfFailed(m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE,
			IID_PPV_ARGS(&mFence)));

		mRtvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		mDsvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		mCbvSrvUavDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		// Check 4X MSAA quality support for our back buffer format.
		// All Direct3D 11 capable devices support 4X MSAA for all render 
		// target formats, so we only need to check quality support.

		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
		msQualityLevels.Format = mBackBufferFormat;
		msQualityLevels.SampleCount = 4;
		msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		msQualityLevels.NumQualityLevels = 0;
		ThrowIfFailed(m_Device->CheckFeatureSupport(
			D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
			&msQualityLevels,
			sizeof(msQualityLevels)));


		return 0;
	}

	void GraphicsMgrD12::Finalize() noexcept
	{

	}

	void GraphicsMgrD12::Tick() noexcept
	{

	}
}