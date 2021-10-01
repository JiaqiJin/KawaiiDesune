#include "../Kawaiipch.h"

#include "RenderBackendImpl.h"

namespace Kawaii
{
    RenderBackendImpl::RenderBackendImpl()
    {
    }

    RenderBackendImpl::~RenderBackendImpl()
    {

    }

    bool RenderBackendImpl::Initialize()
    {
        bool res = m_KawaiiWindow.Init();
      
        return res;
    }

    void RenderBackendImpl::Unload()
    {
        m_KawaiiWindow.Unload();
    }

    bool RenderBackendImpl::Present()
    {
        return true;
    }
}