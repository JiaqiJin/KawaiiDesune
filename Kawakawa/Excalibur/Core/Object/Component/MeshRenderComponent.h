#pragma once

#include "../../../Interface/IComponent.h"

#include <vector>
#include <memory>
#include <string>

namespace Excalibur
{
	class MeshRenderComponent : public IComponent
	{
	public:
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;
		virtual void Render();
	private:
		bool m_Visible;
	};
}