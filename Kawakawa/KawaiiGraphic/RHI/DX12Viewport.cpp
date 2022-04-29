#include "pch.h"
#include "DX12Viewport.h"
#include "DX12GraphicRHI.h"
#include "DX12Device.h"
#include "Texture/TextureInfo.h"

namespace RHI
{
	DX12Viewport::DX12Viewport(DX12GraphicRHI* DX12RHI, const DX12ViewportInfo& Info, int Width, int Height)
		: m_DX12RHI(DX12RHI), m_ViewportInfo(Info), m_ViewportWidth(Width), m_ViewportHeight(Height)
	{
		CreateSwapChain();
	}

	DX12Viewport::~DX12Viewport()
	{

	}

	void DX12Viewport::OnResize(int NewWidth, int NewHeight)
	{
		m_ViewportWidth = NewWidth;
		m_ViewportHeight = NewHeight;

		// Flush before changing any resources.
		m_DX12RHI->GetDevice()->GetCommandContext()->FlushCommandQueue();

		m_DX12RHI->GetDevice()->GetCommandContext()->ResetCommandList();

		// Release the previous resources
		for (UINT i = 0; i < m_SwapChainBufferCount; i++)
		{
			m_RenderTargetTextures[i].reset();
		}
		m_DepthStencilTexture.reset();

		// Resize the swap chain.
		ThrowIfFailed(m_SwapChain->ResizeBuffers(m_SwapChainBufferCount, m_ViewportWidth, m_ViewportHeight,
			m_ViewportInfo.BackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

		m_CurrBackBuffer = 0;

		// Create RenderTargetTextures
		for (UINT i = 0; i < m_SwapChainBufferCount; i++)
		{
			Microsoft::WRL::ComPtr<ID3D12Resource> SwapChainBuffer = nullptr;
			ThrowIfFailed(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&SwapChainBuffer)));

			D3D12_RESOURCE_DESC BackBufferDesc = SwapChainBuffer->GetDesc();

			TextureInfo textureInfo;
			textureInfo.RTVFormat = BackBufferDesc.Format;
			textureInfo.InitState = D3D12_RESOURCE_STATE_PRESENT;
			m_RenderTargetTextures[i] = m_DX12RHI->CreateTexture(SwapChainBuffer, textureInfo, TexCreate_RTV);
		}

		// Create DepthStencilTexture
		TextureInfo textureInfo;
		textureInfo.Type = ETextureType::TEXTURE_2D;
		textureInfo.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureInfo.Width = m_ViewportWidth;
		textureInfo.Height = m_ViewportHeight;
		textureInfo.Depth = 1;
		textureInfo.MipCount = 1;
		textureInfo.ArraySize = 1;
		textureInfo.InitState = D3D12_RESOURCE_STATE_DEPTH_WRITE;
		textureInfo.Format = DXGI_FORMAT_R24G8_TYPELESS;  // Create with a typeless format, support DSV and SRV(for SSAO)
		textureInfo.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		textureInfo.SRVFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

		m_DepthStencilTexture = m_DX12RHI->CreateTexture(textureInfo, TexCreate_DSV | TexCreate_SRV);

		// Execute the resize commands.
		m_DX12RHI->GetDevice()->GetCommandContext()->ExecuteCommandLists();
		// Wait until resize is complete.
		m_DX12RHI->GetDevice()->GetCommandContext()->FlushCommandQueue();
	}

	void DX12Viewport::Present()
	{
		// swap the back and front buffers
		ThrowIfFailed(m_SwapChain->Present(0, 0));
		m_CurrBackBuffer = (m_CurrBackBuffer + 1) % m_SwapChainBufferCount;
	}

	void DX12Viewport::CreateSwapChain()
	{
		// Release the previous swapchain we will be recreating.
		m_SwapChain.Reset();

		DXGI_SWAP_CHAIN_DESC Desc;
		Desc.BufferDesc.Width = m_ViewportWidth;
		Desc.BufferDesc.Height = m_ViewportHeight;
		Desc.BufferDesc.RefreshRate.Numerator = 60;
		Desc.BufferDesc.RefreshRate.Denominator = 1;
		Desc.BufferDesc.Format = m_ViewportInfo.BackBufferFormat;
		Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		Desc.SampleDesc.Count = m_ViewportInfo.bEnable4xMsaa ? 4 : 1;
		Desc.SampleDesc.Quality = m_ViewportInfo.bEnable4xMsaa ? (m_ViewportInfo.QualityOf4xMsaa - 1) : 0;
		Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		Desc.BufferCount = m_SwapChainBufferCount;
		Desc.OutputWindow = m_ViewportInfo.WindowHandle;
		Desc.Windowed = true;
		Desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		Desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// Note: Swap chain uses queue to perform flush.
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> CommandQueue = m_DX12RHI->GetDevice()->GetCommandQueue();

		ThrowIfFailed(m_DX12RHI->GetDxgiFactory()->CreateSwapChain(CommandQueue.Get(), &Desc, m_SwapChain.GetAddressOf()));
	}

	void DX12Viewport::GetD3DViewport(D3D12_VIEWPORT& OutD3DViewPort, D3D12_RECT& OutD3DRect)
	{
		OutD3DViewPort.TopLeftX = 0;
		OutD3DViewPort.TopLeftY = 0;
		OutD3DViewPort.Width = static_cast<float>(m_ViewportWidth);
		OutD3DViewPort.Height = static_cast<float>(m_ViewportHeight);
		OutD3DViewPort.MinDepth = 0.0f;
		OutD3DViewPort.MaxDepth = 1.0f;

		OutD3DRect = { 0, 0, m_ViewportWidth, m_ViewportHeight };
	}
}