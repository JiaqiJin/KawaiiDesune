#pragma once

#include "../Interface/IRuntimeModule.h"

/*
* Responsible for loading resource files. 
* 静态图片（Image）, 动态图像, 音频文件, 3D场景文件
*/

namespace Kawaii::Asset
{
    class AssetManager : public IRuntimeModule
    {
    public:
        int  Initialize() final;
        void Tick() final;
        void Finalize() final;
    };
}  

namespace Kawaii 
{
    extern std::unique_ptr<Asset::AssetManager> g_AssetManager;
}