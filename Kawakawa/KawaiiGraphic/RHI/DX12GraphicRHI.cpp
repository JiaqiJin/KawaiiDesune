#include "pch.h"
#include "DX12GraphicRHI.h"
#include "DX12Device.h"
#include "DX12CommandContext.h"
#include "DX12Texture.h"
#include "DX12MemoryAllocator.h"

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

	// Commands 
	void  DX12GraphicRHI::ExecuteCommandLists()
	{
		GetDevice()->GetCommandContext()->ExecuteCommandLists();
	}

	void  DX12GraphicRHI::FlushCommandQueue()
	{
		GetDevice()->GetCommandContext()->FlushCommandQueue();
	}

	void  DX12GraphicRHI::ResetCommandList()
	{
		GetDevice()->GetCommandContext()->ResetCommandList();
	}

	void  DX12GraphicRHI::ResetCommandAllocator()
	{
		GetDevice()->GetCommandContext()->ResetCommandAllocator();
	}

	void  DX12GraphicRHI::Present()
	{
		GetViewport()->Present();
	}

	void  DX12GraphicRHI::ResizeViewport(int Width, int Height)
	{
		GetViewport()->OnResize(Width, Height);
	}

	void  DX12GraphicRHI::TransitionResource(DX12Resource* Resource, D3D12_RESOURCE_STATES StateAfter)
	{
		D3D12_RESOURCE_STATES StateBefore = Resource->m_CurrentState;

		if (StateBefore != StateAfter)
		{
			GetDevice()->GetCommandList()->ResourceBarrier(1, 
				&CD3DX12_RESOURCE_BARRIER::Transition(Resource->m_Resource.Get(), StateBefore, StateAfter));

			Resource->m_CurrentState = StateAfter;
		}
	}

	void DX12GraphicRHI::CopyResource(DX12Resource* DstResource, DX12Resource* SrcResource)
	{
		GetDevice()->GetCommandList()->CopyResource(DstResource->m_Resource.Get(), SrcResource->m_Resource.Get());
	}

	void DX12GraphicRHI::CopyBufferRegion(DX12Resource* DstResource, UINT64 DstOffset,
		DX12Resource* SrcResource, UINT64 SrcOffset, UINT64 Size)
	{
		GetDevice()->GetCommandList()->CopyBufferRegion(DstResource->m_Resource.Get(),
			DstOffset, SrcResource->m_Resource.Get(), SrcOffset, Size);
	}

	void DX12GraphicRHI::CopyTextureRegion(const D3D12_TEXTURE_COPY_LOCATION* Dst, UINT DstX, 
		UINT DstY, UINT DstZ, const D3D12_TEXTURE_COPY_LOCATION* Src, const D3D12_BOX* SrcBox)
	{
		GetDevice()->GetCommandList()->CopyTextureRegion(Dst, DstX, DstY, DstZ, Src, SrcBox);
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

	void DX12GraphicRHI::UploadTextureData(DX12TextureRef Texture, const std::vector<D3D12_SUBRESOURCE_DATA>& InitData)
	{
		auto TextureResource = Texture->GetResource();
		D3D12_RESOURCE_DESC TexDesc = TextureResource->m_Resource->GetDesc();

		const UINT NumSubresources = (UINT)InitData.size();

		// GetCopyableFootprints
		std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT> Layouts(NumSubresources);
		std::vector<uint32_t> NumRows(NumSubresources);
		std::vector<uint64_t> RowSizesInBytes(NumSubresources);
		uint64_t RequiredSize = 0;

		m_RenderDevice->GetD3DDevice()->GetCopyableFootprints(&TexDesc, 0, NumSubresources, 0, 
			&Layouts[0], &NumRows[0], &RowSizesInBytes[0], &RequiredSize);

		// Create upload resource
		DX12ResourceAllocation UploadResourceAllocation;
		auto UploadBufferAllocator = GetDevice()->GetUploadBufferAllocator();
		void* MappedData = UploadBufferAllocator->AllocUploadResource((uint32_t)RequiredSize, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT, UploadResourceAllocation);
		ID3D12Resource* UploadBuffer = UploadResourceAllocation.m_UnderlyingResource->m_Resource.Get();

		// Copy contents to upload resource
		for (uint32_t i = 0; i < NumSubresources; i++)
		{
			if (RowSizesInBytes[i] > SIZE_T(-1))
			{
				assert(0);
			}
			D3D12_MEMCPY_DEST DestData = { (BYTE*)MappedData + Layouts[i].Offset, Layouts[i].Footprint.RowPitch, SIZE_T(Layouts[i].Footprint.RowPitch) * SIZE_T(NumRows[i]) };
			MemcpySubresource(&DestData, &(InitData[i]), static_cast<SIZE_T>(RowSizesInBytes[i]), NumRows[i], Layouts[i].Footprint.Depth);
		}

		// Copy data from upload resource to default resource
		TransitionResource(TextureResource, D3D12_RESOURCE_STATE_COPY_DEST);

		for (UINT i = 0; i < NumSubresources; ++i)
		{
			Layouts[i].Offset += UploadResourceAllocation.OffsetFromBaseOfResource;

			CD3DX12_TEXTURE_COPY_LOCATION Src;
			Src.pResource = UploadBuffer;
			Src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
			Src.PlacedFootprint = Layouts[i];

			CD3DX12_TEXTURE_COPY_LOCATION Dst;
			Dst.pResource = TextureResource->m_Resource.Get();
			Dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			Dst.SubresourceIndex = i;

			CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);
		}

		TransitionResource(TextureResource, D3D12_RESOURCE_STATE_COMMON);
	}

	void DX12GraphicRHI::CreateTextureViews(DX12TextureRef TextureRef, const TextureInfo& TextureInfo, uint32_t CreateFlags)
	{
		auto TextureResource = TextureRef->GetD3DResource();

		// Create SRV
		if (CreateFlags & TexCreate_SRV)
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC SrvDesc = {};
			SrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

			if (TextureInfo.SRVFormat == DXGI_FORMAT_UNKNOWN)
			{
				SrvDesc.Format = TextureInfo.Format;
			}
			else
			{
				SrvDesc.Format = TextureInfo.SRVFormat;
			}

			if (TextureInfo.Type == ETextureType::TEXTURE_2D)
			{
				SrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				SrvDesc.Texture2D.MostDetailedMip = 0;
				SrvDesc.Texture2D.MipLevels = (uint16_t)TextureInfo.MipCount;
			}
			else if (TextureInfo.Type == ETextureType::TEXTURE_CUBE)
			{
				SrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
				SrvDesc.TextureCube.MostDetailedMip = 0;
				SrvDesc.TextureCube.MipLevels = (uint16_t)TextureInfo.MipCount;
			}
			else
			{
				SrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
				SrvDesc.Texture3D.MostDetailedMip = 0;
				SrvDesc.Texture3D.MipLevels = (uint16_t)TextureInfo.MipCount;
			}

			auto SrvRef = std::make_unique<DX12ShaderResourceView>(GetDevice(), SrvDesc, TextureResource);
			TextureRef->AddSRV(SrvRef);
		}
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