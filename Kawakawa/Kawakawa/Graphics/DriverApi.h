#pragma once

#include "Format.h"

namespace Kawaii::Graphics
{
    enum struct APIType 
    {
        DirectX12,
        Vulkan,
        OpenGL
    };

    class DriverAPI
    {
    public:
        virtual ~DriverAPI() = default;

        virtual void CreateSwapChain(uint32_t width, uint32_t height, unsigned frameCount, Format format, void* window) = 0;
        APIType GetType() const { return m_Type; }

    protected:
        DriverAPI(APIType type) : m_Type(type) {}
        const APIType m_Type;
    };
}