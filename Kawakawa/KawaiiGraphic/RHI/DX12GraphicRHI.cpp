#include "pch.h"
#include "DX12GraphicRHI.h"
#include "DX12Device.h"

using Microsoft::WRL::ComPtr;

namespace RHI
{
	DX12GraphicRHI::DX12GraphicRHI()
	{

	}

	DX12GraphicRHI::~DX12GraphicRHI()
	{
		Destroy();
	}

	void DX12GraphicRHI::Initialize(HWND WindowHandle, int WindowWidth, int WindowHeight)
	{
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
		ThrowIfFailed(CreateDXGIFactory2(DxgiFactoryFlags, IID_PPV_ARGS(m_DxgiFactory.GetAddressOf())));

		// Create Device
		m_RenderDevice = std::make_unique<RenderDevice>(this);

		// Create Viewport
		m_ViewportInfo.WindowHandle = WindowHandle;
		m_ViewportInfo.BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_ViewportInfo.DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		m_ViewportInfo.bEnable4xMsaa = false;
		m_ViewportInfo.QualityOf4xMsaa = GetSupportMSAAQuality(m_ViewportInfo.BackBufferFormat);

		m_Viewport = std::make_unique<DX12Viewport>(this, m_ViewportInfo, WindowWidth, WindowHeight);

#ifdef _DEBUG
		LogAdapters();
#endif
	}

	void DX12GraphicRHI::Destroy()
	{

	}

	// Texture
	DX12TextureRef DX12GraphicRHI::CreateTexture(const TextureInfo& TextureInfo, uint32_t CreateFlags, TVector4 RTVClearValue)
	{
		DX12TextureRef TextureRef = CreateTextureResource(TextureInfo, CreateFlags, RTVClearValue);

		CreateTextureViews(TextureRef, TextureInfo, CreateFlags);

		return TextureRef;
	}

	DX12TextureRef DX12GraphicRHI::CreateTexture(Microsoft::WRL::ComPtr<ID3D12Resource> D3DResource, 
		const TextureInfo& TextureInfo, uint32_t CreateFlags)
	{
		DX12TextureRef TextureRef = std::make_shared<DX12Texture>();

		DX12Resource* NewResource = new DX12Resource(D3DResource, TextureInfo.InitState);
		TextureRef->m_ResourceAllocation.m_UnderlyingResource = NewResource;
		TextureRef->m_ResourceAllocation.SetType(DX12ResourceAllocation::EResourceLocationType::StandAlone);

		CreateTextureViews(TextureRef, TextureInfo, CreateFlags);

		return TextureRef;
	}

	DX12TextureRef DX12GraphicRHI::CreateTextureResource(const TextureInfo& TextureInfo, uint32_t CreateFlags, TVector4 RTVClearValue)
	{
		DX12TextureRef TextureRef = std::make_shared<DX12Texture>();

		//Create default resource
		D3D12_RESOURCE_STATES ResourceState = D3D12_RESOURCE_STATE_COMMON;

		D3D12_RESOURCE_DESC TexDesc = {};
		ZeroMemory(&TexDesc, sizeof(D3D12_RESOURCE_DESC));

		Microsoft::WRL::ComPtr<ID3D12Resource> Resource;
		CD3DX12_CLEAR_VALUE ClearValue = {};
		CD3DX12_CLEAR_VALUE* ClearValuePtr = nullptr;

		GetDevice()->GetD3DDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&TexDesc,
			D3D12_RESOURCE_STATE_COMMON,
			ClearValuePtr,
			IID_PPV_ARGS(&Resource));

		// TODO

		return TextureRef;
	}

	void DX12GraphicRHI::CreateTextureViews(DX12TextureRef TextureRef, const TextureInfo& TextureInfo, uint32_t CreateFlags)
	{
		auto TextureResource = TextureRef->GetD3DResource();


	}

	UINT DX12GraphicRHI::GetSupportMSAAQuality(DXGI_FORMAT BackBufferFormat)
	{
		// Check 4X MSAA quality support for our back buffer format.
		// All Direct3D 11 capable devices support 4X MSAA for all render 
		// target formats, so we only need to check quality support.
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
		msQualityLevels.Format = BackBufferFormat;
		msQualityLevels.SampleCount = 4;
		msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		msQualityLevels.NumQualityLevels = 0;
		ThrowIfFailed(m_RenderDevice->GetD3DDevice()->CheckFeatureSupport(
			D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
			&msQualityLevels,
			sizeof(msQualityLevels)));

		UINT QualityOf4xMsaa = msQualityLevels.NumQualityLevels;
		assert(QualityOf4xMsaa > 0 && "Unexpected MSAA quality level.");

		return QualityOf4xMsaa;
	}

	void DX12GraphicRHI::LogAdapters()
	{
		UINT i = 0;
		IDXGIAdapter* adapter = nullptr;
		std::vector<IDXGIAdapter*> adapterList;
		while (m_DxgiFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_ADAPTER_DESC desc;
			adapter->GetDesc(&desc);

			std::wstring text = L"***Adapter: ";
			text += desc.Description;
			text += L"\n";

			OutputDebugString(text.c_str());

			adapterList.push_back(adapter);

			++i;
		}

		for (size_t i = 0; i < adapterList.size(); ++i)
		{
			LogAdapterOutputs(adapterList[i]);
			ReleaseCom(adapterList[i]);
		}
	}

	void DX12GraphicRHI::LogAdapterOutputs(IDXGIAdapter* adapter)
	{
		UINT i = 0;
		IDXGIOutput* output = nullptr;
		while (adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_OUTPUT_DESC desc;
			output->GetDesc(&desc);

			std::wstring text = L"***Output: ";
			text += desc.DeviceName;
			text += L"\n";
			OutputDebugString(text.c_str());

			LogOutputDisplayModes(output, m_ViewportInfo.BackBufferFormat);

			ReleaseCom(output);

			++i;
		}
	}

	void DX12GraphicRHI::LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format)
	{
		UINT count = 0;
		UINT flags = 0;

		// Call with nullptr to get list count.
		output->GetDisplayModeList(format, flags, &count, nullptr);

		std::vector<DXGI_MODE_DESC> modeList(count);
		output->GetDisplayModeList(format, flags, &count, &modeList[0]);

		for (auto& x : modeList)
		{
			UINT n = x.RefreshRate.Numerator;
			UINT d = x.RefreshRate.Denominator;
			std::wstring text =
				L"Width = " + std::to_wstring(x.Width) + L" " +
				L"Height = " + std::to_wstring(x.Height) + L" " +
				L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
				L"\n";

			::OutputDebugString(text.c_str());
		}
	}
}