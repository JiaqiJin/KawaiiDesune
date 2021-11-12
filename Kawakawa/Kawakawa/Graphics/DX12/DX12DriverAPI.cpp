#include "D3Dpch.h"
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
}