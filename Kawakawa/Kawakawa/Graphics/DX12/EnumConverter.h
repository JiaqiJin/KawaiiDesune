#pragma once

namespace Kawaii::Graphics::backend::DX12
{
    inline DXGI_FORMAT index_size_to_dxgi_format(size_t size)
    {
        if (size == 4) {
            return DXGI_FORMAT_R32_UINT;
        }
        else if (size == 2) {
            return DXGI_FORMAT_R16_UINT;
        }
        else if (size == 1) {
            return DXGI_FORMAT_R8_UINT;
        }
        else {
            return DXGI_FORMAT_UNKNOWN;
        }
    }
}