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
		m_RenderDevice = std::make_unique<RenderDevice>();
	}

	// Texture
	DX12TextureRef DX12GraphicRHI::CreateTexture(uint32_t CreateFlags, Math::Vector4 RTVClearValue)
	{
		DX12TextureRef TextureRef = CreateTextureResource(CreateFlags, RTVClearValue);

		CreateTextureViews(TextureRef, CreateFlags);

		return TextureRef;
	}

	DX12TextureRef DX12GraphicRHI::CreateTextureResource(uint32_t CreateFlags, Math::Vector4 RTVClearValue = Math::Vector4(0.f))
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

	void DX12GraphicRHI::CreateTextureViews(DX12TextureRef TextureRef, uint32_t CreateFlags)
	{

	}
}