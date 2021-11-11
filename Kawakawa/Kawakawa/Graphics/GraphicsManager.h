#pragma once

#include "../Interface/IRuntimeModule.h"

namespace Kawaii::Graphics
{
	class GraphicsManager : public IRuntimeModule
	{
	public:
		int  Initialize() final;
		void Finalize() final;
		void Tick() final;

	protected:
	};
}

namespace Kawaii 
{
	extern std::unique_ptr<Graphics::GraphicsManager> g_GraphicsManager;
}