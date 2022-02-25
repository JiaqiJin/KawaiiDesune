#include "MeshRenderComponent.h"
#include "../Entity.h"
#include "../World.h"

namespace Excalibur
{
	int MeshRenderComponent::Initialize()
	{
		auto master = GetMaster();
		if (master) {
			auto world = master->GetWorld();
			if (world) {
				world->GetMeshRenderSystem()->AddComponent(this);
			}
		}
		return 0;
	}

	void MeshRenderComponent::Finalize()
	{
		auto master = GetMaster();
		if (master) {
			auto world = master->GetWorld();
			if (world) {
				world->GetMeshRenderSystem()->DeleteComponent(this);
			}
		}
	}

}