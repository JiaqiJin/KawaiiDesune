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

		// Commands
		void ExecuteCommandLists();
		void FlushCommandQueue();
		void ResetCommandList();
		void ResetCommandAllocator();

		void Present();
		void TransitionResource(DX12Resource* Resource, D3D12_RESOURCE_STATES StateAfter);
		void ResizeViewport(int Width, int Height);

		void CopyResource(DX12Resource* DstResource, DX12Resource* SrcResource);
		void CopyBufferRegion(DX12Resource* DstResource, UINT64 DstOffset, DX12Resource* SrcResource, UINT64 SrcOffset, UINT64 Size);
		void CopyTextureRegion(const D3D12_TEXTURE_COPY_LOCATION* Dst, UINT DstX, UINT DstY, UINT DstZ, const D3D12_TEXTURE_COPY_LOCATION* Src, const D3D12_BOX* SrcBox);

		// Buffers Creation
		DX12ConstantBufferRef CreateConstantBuffer(const void* Data, uint32_t Size);
		DX12VertexBufferRef CreateVertexBuffer(const void* Data, uint32_t Size);
		DX12IndexBufferRef CreateIndexBuffer(const void* Data, uint32_t Size);
		DX12ReadBackBufferRef CreateReadBackBuffer(uint32_t Size);

		// Texture
		DX12TextureRef CreateTexture(const TextureInfo& TextureInfo, uint32_t CreateFlags, TVector4 RTVClearValue = TVector4::Zero);
		DX12TextureRef CreateTexture(Microsoft::WRL::ComPtr<ID3D12Resource> D3DResource, const TextureInfo& TextureInfo, uint32_t CreateFlags);

		void UploadTextureData(DX12TextureRef Texture, const std::vector<D3D12_SUBRESOURCE_DATA>& InitData);

		// Getters
		RenderDevice* GetDevice() { return m_RenderDevice.get(); }
		IDXGIFactory4* GetDxgiFactory() { return m_DxgiFactory.Get(); }
		DX12Viewport* GetViewport() { return m_Viewport.get(); }
		const DX12ViewportInfo& GetViewportInfo() { return m_ViewportInfo; }
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