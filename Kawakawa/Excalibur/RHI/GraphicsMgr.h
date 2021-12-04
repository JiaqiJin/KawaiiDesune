#pragma once

#include "../Interface/IModule.h"

namespace Excalibur
{
	class GraphicsManager : public ITickableModule
	{
	public:
		virtual	int	Initialize() noexcept = 0;
		virtual	void Finalize() noexcept = 0;
	protected:

	};
}