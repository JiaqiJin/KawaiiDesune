#pragma once

namespace Kawaii
{
    class RenderBackend
    {
    public:
        virtual ~RenderBackend() = default;

        virtual bool Initialize() = 0;
        virtual void Unload() = 0;
        virtual bool Present() = 0;

        static RenderBackend* CreateRenderBackend();
        static void DeleteRenderBackend(RenderBackend* object);
    };
}