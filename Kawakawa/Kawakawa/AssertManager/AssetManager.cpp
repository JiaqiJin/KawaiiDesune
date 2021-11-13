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

    
        return 0;
    }

    void AssetManager::Tick() {}

    void AssetManager::Finalize() 
    {
        m_Logger->info("Finalized.");
        m_Logger = nullptr;
    }
}