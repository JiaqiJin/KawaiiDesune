#pragma once

#include "DX12Texture.h"

namespace RHI
{
	class RenderDevice;

	class DX12GraphicRHI
	{
	public:
		DX12GraphicRHI();
		~DX12GraphicRHI();

		void Initialize(HWND WindowHandle, int WindowWidth, int WindowHeight);

		DX12TextureRef CreateTexture(uint32_t CreateFlags, Math::Vector4 RTVClearValue = Math::Vector4(0.f));

		// Getters
		RenderDevice* GetDevice() { return m_RenderDevice.get(); }

	private:
		DX12TextureRef CreateTextureResource(uint32_t CreateFlags, Math::Vector4 RTVClearValue);
		void CreateTextureViews(DX12TextureRef TextureRef, uint32_t CreateFlags);
	private:
		std::unique_ptr<RenderDevice> m_RenderDevice = nullptr;
	};
}