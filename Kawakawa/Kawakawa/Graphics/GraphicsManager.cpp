#include "GraphicsManager.h"
#include "../Platform/Application.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "DX12/DX12DriverAPI.h"

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

		// Init API
		m_Driver = std::make_unique<backend::DX12::DX12DriverAPI>();
		// TODO

		// Init Frame
		m_Driver->CreateSwapChain(config.screenWidth, config.screenHeight, sm_SwapChianSize, Format::R8G8B8A8_UNORM, g_App->GetWindow());

		// TODO
		 
		return 0;
	}

	void GraphicsManager::Finalize()
	{
		m_Logger->info("Finalized.");

		{
			m_Driver->IdleGPU();
			// TODO
		}

		m_Driver = nullptr;
	}

	void GraphicsManager::Tick()
	{
		// TODO

		m_Driver->Present(m_CurrBackBuffer);
		m_CurrBackBuffer = (m_CurrBackBuffer + 1) % sm_SwapChianSize;
	}
}