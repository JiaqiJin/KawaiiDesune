#pragma once

#include "../Interface/IRuntimeModule.h"

namespace Kawaii::Asset
{
    class AssetManager : public IRuntimeModule
    {
    public:
        int  Initialize() final;
        void Tick() final;
        void Finalize() final;
    };
}  // namespace Hitagi::Asset

namespace Kawaii 
{
    extern std::unique_ptr<Asset::AssetManager> g_AssetManager;
}