#pragma once

#include "../../../Interface/IComponent.h"
#include "../../../RHI/RenderObject.h"

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
		virtual void Render();

		std::shared_ptr<RenderObject> AddRenderObject();
		std::shared_ptr<RenderObject> GetRenderObject(int idx);
		size_t GetRenderObjectCount();

		void SetVisible(bool v) { m_Visible = v; }
		bool IsVisible() { return m_Visible; }
	private:
		bool m_Visible;
		std::vector<std::shared_ptr<RenderObject>> mRenderObjects;
	};
}