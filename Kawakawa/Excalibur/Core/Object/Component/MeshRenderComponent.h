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
		MeshRenderComponent() : m_Visible(true) {}

		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;

		void SetVisible(bool v) { m_Visible = v; }
		bool IsVisible() { return m_Visible; }
	public:
		bool m_Visible;
		std::vector<int> mMeshIdxes;
	};
}