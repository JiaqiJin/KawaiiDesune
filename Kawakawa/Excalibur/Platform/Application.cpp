#include "Application.h"
#include <iostream>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Excalibur
{
	int Application::Initialize() noexcept 
	{
		m_Logger = spdlog::stdout_color_mt("Application");
#if defined(_DEBUG)
		spdlog::set_level(spdlog::level::debug);
#endif  // _DEBUG
		mMemoryMgr = nullptr;
		mQuit = false;
		return 0;
	}

	void Application::Tick() noexcept 
	{

	}

	void Application::Render() noexcept
	{

	}

	void Application::Run() noexcept 
	{
		while (!IsQuit())
		{
			Tick();
			Render();
		}
	}

	void Application::Finalize() noexcept 
	{

	}

	bool Application::IsQuit() noexcept
	{
		return mQuit;
	}

	void Application::Quit() noexcept
	{
		mQuit = true;
	}
}