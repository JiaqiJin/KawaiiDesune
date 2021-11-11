#include "GraphicsManager.h"
#include "../Platform/Application.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Kawaii 
{
	std::unique_ptr<Graphics::GraphicsManager> g_GraphicsManager = std::make_unique<Graphics::GraphicsManager>();
}

namespace Kawaii::Graphics
{
	int GraphicsManager::Initialize()
	{
		m_Logger = spdlog::stdout_color_mt("GraphicsManager");
		m_Logger->info("Initialize...");
		auto& config = g_App->GetConfiguration();
		// TODO
		return 0;
	}

	void GraphicsManager::Finalize()
	{
		m_Logger->info("Finalized.");
		// TODO
	}

	void GraphicsManager::Tick()
	{
		// TODO
	}
}