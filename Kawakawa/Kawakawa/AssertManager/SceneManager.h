#pragma once

#include "../Interface/IRuntimeModule.h"

namespace Kawaii::Asset
{
	class SceneManager : public IRuntimeModule
	{
	public:
		int  Initialize() override;
		void Finalize() override;
		void Tick() override;

	protected:

	};

}


namespace Kawaii
{
	extern std::unique_ptr<Asset::SceneManager> g_SceneManager;
}