#pragma once

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

        APIType GetType() const { return m_Type; }

    protected:
        DriverAPI(APIType type) : m_Type(type) {}
        const APIType m_Type;
    };
}