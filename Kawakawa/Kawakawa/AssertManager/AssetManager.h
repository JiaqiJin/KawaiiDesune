#pragma once

#include "../Interface/IRuntimeModule.h"

#include "Parser/ImageParser.h"

#include <filesystem>
#include <array>
#include <memory>

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

        Image ParseImage(const std::filesystem::path& path) const;
    private:
        std::array<std::unique_ptr<ImageParser>, static_cast<size_t>(ImageFormat::NUM_SUPPORT)> m_ImageParser;
    };
}  

namespace Kawaii 
{
    extern std::unique_ptr<Asset::AssetManager> g_AssetManager;
}