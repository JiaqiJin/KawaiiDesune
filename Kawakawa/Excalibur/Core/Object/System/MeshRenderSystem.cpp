#include "MeshRenderSystem.h"

#include "../../Object/World.h"

namespace Excalibur
{
	MeshRenderSystem::MeshRenderSystem(World* world)
		: mWorld(world)
	{

	}
}

int Excalibur::MeshRenderSystem::Initialize() noexcept
{
	mGraphicsManager = mWorld->mApp->mGraphicsManager;
	return 0;
}

void Excalibur::MeshRenderSystem::Finalize() noexcept
{
}

void Excalibur::MeshRenderSystem::Tick() noexcept
{
}

void Excalibur::MeshRenderSystem::AddComponent(MeshRenderComponent* comp)
{
	mComponents.insert(comp);
}

void Excalibur::MeshRenderSystem::DeleteComponent(MeshRenderComponent* comp)
{
	mComponents.erase(comp);
}

void Excalibur::MeshRenderSystem::LoadMesh(aiMesh* mesh, const aiScene* world)
{
	auto mesh_ = mGraphicsManager->CreateRenderMesh(mesh, world);
	mMeshes.push_back(mesh_);
}

void Excalibur::MeshRenderSystem::Render()
{

	for (auto comp : mComponents) {
		if (comp->IsVisible()) {

			auto transform = comp->GetMaster()->GetComponent<TransformComponent>();
			for (auto mesh : comp->mMeshes) {
				auto entity = comp->GetMaster();
				mesh->Render(entity);
			}
		}
	}
}