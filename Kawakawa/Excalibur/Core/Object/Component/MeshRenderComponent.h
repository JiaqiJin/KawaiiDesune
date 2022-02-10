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
		MeshRenderComponent() : m_Visible(true) {}

		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;

		bool IsVisible() { return m_Visible; }
		void SetVisible(bool value) { m_Visible = value; }

	public:
		std::vector<int> m_MeshIndex;

	private:
		bool m_Visible;
	};
}