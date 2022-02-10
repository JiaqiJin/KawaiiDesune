#include "MeshRenderSystem.h"
#include "../../../Platform/Application.h"
#include "../../Object/World.h"

namespace Excalibur
{
	MeshRenderSystem::MeshRenderSystem(World* world)
		:m_World(world)
	{

	}

	int MeshRenderSystem::Initialize() noexcept
	{
		m_GraphicsManager = m_World->m_App->mGraphicsManager;
		return 0;
	}

	void MeshRenderSystem::Finalize() noexcept
	{
		m_Meshes.clear();
	}

	void MeshRenderSystem::Tick() noexcept
	{

	}

	void MeshRenderSystem::AddComponent(MeshRenderComponent* comp)
	{
		m_Components.insert(comp);
	}

	void MeshRenderSystem::DeleteComponent(MeshRenderComponent* comp)
	{
		m_Components.erase(comp);
	}

	void MeshRenderSystem::Render()
	{
		if (!IsActive())
			return;

		for (auto comp : m_Components)
		{
			if (comp->IsVisible()) {

				auto transform = comp->GetMaster()->GetComponent<TransformComponent>();

				for (auto mid : comp->m_MeshIndex) {
					auto mesh = m_Meshes[mid];
					if (mesh) {
						mesh->Render(m_World, transform->GetWorldMatrix());
					}
				}
			}
		}
	}

	void MeshRenderSystem::LoadMesh(aiMesh* mesh, const aiScene* scene)
	{
		auto localMesh = m_GraphicsManager->CreateRenderMesh(mesh, scene);
		m_Meshes.push_back(localMesh);
	}
}