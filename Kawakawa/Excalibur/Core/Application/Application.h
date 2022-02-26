#pragma once

#include "IApplication.h"
#include "../Memory/MemoryManager.h"
#include "../../RHI/GraphicsMgr.h"
#include "../Object/World.h"

namespace Excalibur 
{
	class Application : public IApplication 
	{
	public:
		virtual int	Initialize();
		virtual void Tick();
		virtual void Render();
		virtual void Run();
		virtual void Finalize();
		virtual void Quit();
		virtual bool IsQuit();

	public:
		MemoryManager*		m_MemoryMgr;
		IGraphicsManager*	m_GraphicsManager;
		World*				m_World;

	private:
		bool m_Quit;
	};

	extern Application* GApp;
}