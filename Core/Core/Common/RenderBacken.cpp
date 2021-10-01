#include "../Kawaiipch.h"

#include "RenderBacken.h"

#include "RenderBackendImpl.h"

namespace Kawaii
{
    RenderBackend* RenderBackend::CreateRenderBackend()
    {
        return new RenderBackendImpl();
    }

    void RenderBackend::DeleteRenderBackend(RenderBackend* object)
    {
        delete object;
    }
}