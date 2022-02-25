#include "MeshRenderSystem.h"
#include "../World.h"
#include "../../Application/Application.h"

namespace Excalibur
{
	MeshRenderSystem::MeshRenderSystem(World* world) :
		m_World(world)
	{

	}

	int MeshRenderSystem::Initialize()
	{
		m_GraphicsManager = m_World->mApp->m_GraphicsManager;
		return 0;
	}

	void MeshRenderSystem::Finalize()
	{
		m_Meshes.clear();
	}

	void MeshRenderSystem::Tick()
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
			if (comp->IsVisible()) 
			{

				auto transform = comp->GetMaster()->GetComponent<TransformComponent>();

				for (auto mid : comp->m_MeshIdxes) {
					auto mesh = m_Meshes[mid];
					if (mesh) {
						mesh->Render(m_World, transform->GetWorldMatrix());
					}
				}
			}
		}
	}

	void MeshRenderSystem::LoadMesh(aiMesh* mesh, const aiScene* world)
	{
		auto _mesh = m_GraphicsManager->CreateRenderMesh(mesh, world);
		m_Meshes.push_back(_mesh);
	}

}