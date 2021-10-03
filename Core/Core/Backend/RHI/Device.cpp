#include "../../Kawaiipch.h"

#include "Device.h"

namespace Kawaii
{
    Device::Device()
        : m_Device(nullptr)
    {
    }

    Device::~Device()
    {
       
    }

    bool Device::Initialize()
    {
        HRESULT hr = ::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_Device));
        if (FAILED(hr))
            return false;

        return true;
    }
}