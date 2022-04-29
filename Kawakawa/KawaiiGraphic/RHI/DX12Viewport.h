#pragma once

#include "DX12Texture.h"

namespace RHI
{
	class DX12GraphicRHI;

	struct DX12ViewportInfo
	{
		HWND WindowHandle;

		DXGI_FORMAT BackBufferFormat;
		DXGI_FORMAT DepthStencilFormat;

		bool bEnable4xMsaa = false;    // 4X MSAA enabled
		UINT QualityOf4xMsaa = 0;      // quality level of 4X MSAA
	};

	class DX12Viewport
	{
	public:
		DX12Viewport(DX12GraphicRHI* DX12RHI, const DX12ViewportInfo& Info, int Width, int Height);
		~DX12Viewport();

		void OnResize(int NewWidth, int NewHeight);
		void Present();

		void GetD3DViewport(D3D12_VIEWPORT& OutD3DViewPort, D3D12_RECT& OutD3DRect);
	private:
		void CreateSwapChain();

	private:
		DX12GraphicRHI* m_DX12RHI = nullptr;

		DX12ViewportInfo m_ViewportInfo;

		int m_ViewportWidth = 0;
		int m_ViewportHeight = 0;	

		// Swap Chain 
		static const int m_SwapChainBufferCount = 2;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain = nullptr;
		int m_CurrBackBuffer = 0;

		DX12TextureRef m_RenderTargetTextures[m_SwapChainBufferCount];
		DX12TextureRef m_DepthStencilTexture = nullptr;
	};
}