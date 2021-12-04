#include "Application.h"
#include <iostream>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Excalibur
{
	int Application::Initialize() noexcept 
	{
		mQuit = false;
		mMemoryMgr = nullptr;
		mGraphicsManager = nullptr;
		mWorld = nullptr;
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