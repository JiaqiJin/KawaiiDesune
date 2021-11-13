#include "SceneManager.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Kawaii
{
	std::unique_ptr<Asset::SceneManager> g_SceneManager = std::make_unique<Asset::SceneManager>();
}

namespace Kawaii::Asset
{
	int SceneManager::Initialize()
	{
		int result = 0;
		m_Logger = spdlog::stdout_color_mt("SceneManager");
		m_Logger->info("Initialize...");

		return result;
	}

	void SceneManager::Finalize()
	{

	}

	void SceneManager::Tick()
	{

	}
}