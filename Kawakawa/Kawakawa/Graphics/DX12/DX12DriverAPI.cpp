#include "DX12DriverAPI.h"

#include <windef.h>

namespace Kawaii::Graphics::backend::DX12
{
    DX12DriverAPI::DX12DriverAPI()
		: DriverAPI(APIType::DirectX12),
		m_CommandManager(m_Device.Get())
	{
        unsigned dxgiFactoryFlags = 0;

#if defined(_DEBUG)

        // Enable d3d12 debug layer.
        {
            ComPtr<ID3D12Debug3> debugController;
            if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
                debugController->EnableDebugLayer();
                // Enable additional debug layers.
                dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
            }
        }
#endif  // DEBUG
        ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_DxgiFactory)));

        // Create device.
        {
            if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_1, IID_PPV_ARGS(&m_Device))))
            {
                ComPtr<IDXGIAdapter4> pWarpaAdapter;
                ThrowIfFailed(m_DxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpaAdapter)));
                ThrowIfFailed(D3D12CreateDevice(pWarpaAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_Device)));
            }
        }

        // TODO
	}

    DX12DriverAPI::~DX12DriverAPI()
    {
        m_CommandManager.IdleGPU();
    }

    void DX12DriverAPI::CreateSwapChain(uint32_t width, uint32_t height, unsigned frameCount, Format format, void* window)
    {
        auto hWnd = *reinterpret_cast<HWND*>(window);

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.BufferCount = frameCount;
        swapChainDesc.Width = width;
        swapChainDesc.Height = height;
        swapChainDesc.Format = ToDxgiFormat(format);
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
        ThrowIfFailed(m_DxgiFactory->CreateSwapChainForHwnd(m_CommandManager.GetCommandQueue(), hWnd, &swapChainDesc,
            nullptr, nullptr, &swapChain));

        ThrowIfFailed(swapChain.As(&m_SwapChain));
        ThrowIfFailed(m_DxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

        m_Viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, width, height);
    }

    void DX12DriverAPI::Present(size_t frameIndex)
    {
        ThrowIfFailed(m_SwapChain->Present(0, 0));
    }

    // Fence
    void DX12DriverAPI::WaitFence(uint64_t fenceValue)
    {
        m_CommandManager.WaitForFence(fenceValue);
    }

    void DX12DriverAPI::IdleGPU()
    {
        m_CommandManager.IdleGPU();
    }
}