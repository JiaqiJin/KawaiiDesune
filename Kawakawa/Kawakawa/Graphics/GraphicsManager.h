#pragma once

#include "../Interface/IRuntimeModule.h"

#include "DriverApi.h"

namespace Kawaii::Graphics
{
	class GraphicsManager : public IRuntimeModule
	{
	public:
		int  Initialize() final;
		void Finalize() final;
		void Tick() final;

	protected:
		std::unique_ptr<DriverAPI> m_Driver;

		constexpr static uint8_t sm_SwapChianSize = 2;
		int                      m_CurrBackBuffer = 0;
	};
}

namespace Kawaii 
{
	extern std::unique_ptr<Graphics::GraphicsManager> g_GraphicsManager;
}