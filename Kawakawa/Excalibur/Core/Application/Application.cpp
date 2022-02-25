#include "Application.h"

namespace Excalibur
{
	int Application::Initialize() 
	{
		m_Quit = false;
		m_MemoryMgr = nullptr;
		m_GraphicsManager = nullptr;
		m_World = nullptr;
		return 0;
	}


	void Application::Tick()
	{

	}

	void Application::Render() 
	{

	}

	void Application::Run() 
	{
		while (!IsQuit())
		{
			Tick();
			Render();
		}
	}

	void Application::Finalize()
	{
	}

	bool Application::IsQuit() 
	{
		return m_Quit;
	}

	void Application::Quit() 
	{
		m_Quit = true;
	}
}