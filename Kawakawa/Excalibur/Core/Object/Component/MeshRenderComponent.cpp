#include "MeshRenderComponent.h"
#include "../Entity.h"
#include "../World.h"

namespace Excalibur
{
	int MeshRenderComponent::Initialize() noexcept
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

	void MeshRenderComponent::Finalize() noexcept
	{
		auto master = GetMaster();
		if (master) {
			auto world = master->GetWorld();
			if (world) {
				world->GetMeshRenderSystem()->DeleteComponent(this);
			}
		}
	}

	void MeshRenderComponent::Render()
	{
		for (auto mesh : mRenderObjects) {
			if (mesh->IsVisible()) {
				mesh->Render();
			}
		}
	}

	std::shared_ptr<RenderObject> MeshRenderComponent::AddRenderObject()
	{
		// todo: create a RenderObject by resource path.
		auto renderObject = std::make_shared<RenderObject>();
		renderObject->SetVisible(true);
		mRenderObjects.push_back(renderObject);
		return renderObject;
	}

	std::shared_ptr<RenderObject> MeshRenderComponent::GetRenderObject(int idx)
	{
		return mRenderObjects[idx];
	}


	size_t MeshRenderComponent::GetRenderObjectCount()
	{
		return mRenderObjects.size();
	}
}