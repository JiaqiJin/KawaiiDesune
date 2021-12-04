#pragma once

#include "IApplication.h"
#include "../Core/Memory/MemoryManager.h"
#include "../RHI/GraphicsMgr.h"
#include "../Core/Object/World.h"

namespace Excalibur
{
	class Application : public IApplication 
	{

	public:
		virtual int Initialize() noexcept;
		virtual void Tick() noexcept;
		virtual void Render() noexcept;
		virtual void Run() noexcept;
		virtual void Finalize() noexcept;
		virtual void Quit() noexcept;
		virtual bool IsQuit() noexcept;

	public:
		MemoryManager* mMemoryMgr;
		GraphicsManager* mGraphicsManager;
		World* mWorld;
	private:
		bool mQuit;
	};
}