#pragma once

#include "RenderBacken.h"
#include "../Window/KawaiiWindow.h"

namespace Kawaii
{
	class RenderBackendImpl : public RenderBackend
	{
	public:
	public:
		RenderBackendImpl();
		~RenderBackendImpl();

		bool Initialize() final;
		void Unload() final;
		bool Present() final;

	private:
		KawaiiWindow m_KawaiiWindow;
	};
}