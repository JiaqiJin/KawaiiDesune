
#pragma once
#include <set>
#include <crossguid/guid.hpp>
#include "../../../Interface/ISystem.h"
#include "../Component/MeshRenderComponent.h"

using namespace xg;

using namespace xg;

namespace Excalibur
{

	class MeshRenderSystem : public ITickableSystem {

	public:
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;
		virtual void Tick() noexcept;

	public:
		virtual void AddComponent(MeshRenderComponent* comp);
		virtual void DeleteComponent(MeshRenderComponent* comp);
		virtual void Render();

	private:
		std::set< MeshRenderComponent*> mComponents;

	};
}