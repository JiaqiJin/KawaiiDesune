#pragma once

#include "../GraphicsMgr.h"

namespace Excalibur
{
	class GraphicsMgrGL : public GraphicsManager
	{
	public:
		virtual	int	Initialize() noexcept;
		virtual	void Finalize() noexcept;
		virtual	void Tick() noexcept;
	};
}