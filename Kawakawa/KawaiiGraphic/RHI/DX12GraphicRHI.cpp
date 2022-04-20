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

	}

	void DX12GraphicRHI::Initialize(HWND WindowHandle, int WindowWidth, int WindowHeight)
	{
		// Create Device
		m_RenderDevice = std::make_unique<RenderDevice>(this);
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
}