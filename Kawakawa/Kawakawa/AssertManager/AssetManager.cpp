#include "AssetManager.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Kawaii
{
	std::unique_ptr<Asset::AssetManager> g_AssetManager = std::make_unique<Asset::AssetManager>();
}

namespace Kawaii::Asset
{
    int AssetManager::Initialize()
    {
        m_Logger = spdlog::stdout_color_mt("AssetManager");
        m_Logger->info("Initialize...");

        // TODO
        // Image Parser Format(PNG, JPG ...).

        return 0;
    }

    void AssetManager::Tick() {}

    void AssetManager::Finalize() 
    {
        m_Logger->info("Finalized.");
        m_Logger = nullptr;
    }

    Image AssetManager::ParseImage(const std::filesystem::path& path) const
    {
        ImageFormat format = ImageFormat::NUM_SUPPORT;

        auto ext = path.extension();
        if (ext == ".jpeg" || ext == ".jpg")
            format = ImageFormat::JPEG;
        else if (ext == ".bmp")
            format = ImageFormat::BMP;
        else if (ext == ".tga")
            format = ImageFormat::TGA;
        else if (ext == ".png")
            format = ImageFormat::PNG;

        if (format >= ImageFormat::NUM_SUPPORT)
        {
            m_Logger->error("Unkown image format, and return a empty image");
        }
        
        // TODO
        return Image{};
    }
}