#pragma once

#include "DX12Texture.h"
#include "DX12Buffer.h"
#include "Texture/TextureInfo.h"

namespace RHI
{
	class RenderDevice;

	class DX12GraphicRHI
	{
	public:
		DX12GraphicRHI();
		~DX12GraphicRHI();

		void Initialize(HWND WindowHandle, int WindowWidth, int WindowHeight);

		DX12ConstantBufferRef CreateConstantBuffer(const void* Data, uint32_t Size);
		DX12VertexBufferRef CreateVertexBuffer(const void* Data, uint32_t Size);
		DX12IndexBufferRef CreateIndexBuffer(const void* Data, uint32_t Size);
		DX12ReadBackBufferRef CreateReadBackBuffer(uint32_t Size);

		// Texture
		DX12TextureRef CreateTexture(const TextureInfo& TextureInfo, uint32_t CreateFlags, TVector4 RTVClearValue = TVector4::Zero);
		DX12TextureRef CreateTexture(Microsoft::WRL::ComPtr<ID3D12Resource> D3DResource, const TextureInfo& TextureInfo, uint32_t CreateFlags);

		// Getters
		RenderDevice* GetDevice() { return m_RenderDevice.get(); }

	private:
		DX12TextureRef CreateTextureResource(const TextureInfo& TextureInfo,  uint32_t CreateFlags, TVector4 RTVClearValue);
		void CreateTextureViews(DX12TextureRef TextureRef, const TextureInfo& TextureInfo, uint32_t CreateFlags);
	private:
		std::unique_ptr<RenderDevice> m_RenderDevice = nullptr;
	};
}