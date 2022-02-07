#pragma once

#include "../../../Interface/IComponent.h"

#include <vector>
#include <memory>
#include <string>

#include "../../../RHI/MeshBase.h"

namespace Excalibur
{
	class MeshRenderComponent : public IComponent
	{
	public:
		MeshRenderComponent() {}

		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;

	private:
		std::vector<int> m_MeshIndex;
	};
}