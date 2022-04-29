#pragma once

#include "DX12Texture.h"
#include "DX12Buffer.h"
#include "Texture/TextureInfo.h"
#include "DX12Viewport.h"

namespace RHI
{
	class RenderDevice;

	class DX12GraphicRHI
	{
	public:
		DX12GraphicRHI();
		~DX12GraphicRHI();

		void Initialize(HWND WindowHandle, int WindowWidth, int WindowHeight);
		void Destroy();

		DX12ConstantBufferRef CreateConstantBuffer(const void* Data, uint32_t Size);
		DX12VertexBufferRef CreateVertexBuffer(const void* Data, uint32_t Size);
		DX12IndexBufferRef CreateIndexBuffer(const void* Data, uint32_t Size);
		DX12ReadBackBufferRef CreateReadBackBuffer(uint32_t Size);

		// Texture
		DX12TextureRef CreateTexture(const TextureInfo& TextureInfo, uint32_t CreateFlags, TVector4 RTVClearValue = TVector4::Zero);
		DX12TextureRef CreateTexture(Microsoft::WRL::ComPtr<ID3D12Resource> D3DResource, const TextureInfo& TextureInfo, uint32_t CreateFlags);

		// Getters
		RenderDevice* GetDevice() { return m_RenderDevice.get(); }
		IDXGIFactory4* GetDxgiFactory() { return m_DxgiFactory.Get(); }
	private:
		DX12TextureRef CreateTextureResource(const TextureInfo& TextureInfo,  uint32_t CreateFlags, TVector4 RTVClearValue);
		void CreateTextureViews(DX12TextureRef TextureRef, const TextureInfo& TextureInfo, uint32_t CreateFlags);

		UINT GetSupportMSAAQuality(DXGI_FORMAT BackBufferFormat);

		void LogAdapters();
		void LogAdapterOutputs(IDXGIAdapter* adapter);
		void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);
	private:
		std::unique_ptr<RenderDevice> m_RenderDevice = nullptr;
		DX12ViewportInfo m_ViewportInfo;
		std::unique_ptr<DX12Viewport> m_Viewport = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory4> m_DxgiFactory = nullptr;
	};
}