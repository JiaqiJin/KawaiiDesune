#pragma once
#include <vector>
#include <memory>
#include <string>
#include "../../../Interface/IComponent.h"
#include "../../Math/KawaiiMath.h"

namespace Excalibur 
{
	class MeshRenderComponent : public IComponent 
	{
	public:
		MeshRenderComponent() : m_Visible(true){}

		virtual int Initialize();
		virtual void Finalize();
	public:
		void SetVisible(bool v) { m_Visible = v; }
		bool IsVisible() { return m_Visible; }

	public:
		std::vector<int> m_MeshIdxes;
		bool m_Visible;

	};

}