#include "World.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include "../../Platform/Application.h"

namespace Excalibur
{
	World::World(Application* app)
		: m_App(app), 
		m_MeshRenderSystem(nullptr)
	{

	}

	int	World::Initialize() noexcept
	{
		m_MeshRenderSystem = new MeshRenderSystem(this);
		m_MeshRenderSystem->Initialize();

		m_CameraSystem = new CameraSystem(this);
		m_CameraSystem->Initialize();

		return 0;
	}

	void World::Finalize() noexcept
	{
		m_Entities.clear();

		m_MeshRenderSystem->Finalize();
		m_CameraSystem->Finalize();
	}

	void World::Tick() noexcept
	{

	}

	void World::Render()
	{
		// Render
		m_App->mGraphicsManager->ClearRenderTarget(0.2f, 0.4f, 0.6f, 1.0f);
		m_MeshRenderSystem->Render();
		m_App->mGraphicsManager->Present();
	}

	std::shared_ptr<Entity> World::CreateEntity()
	{
		auto entity = std::make_shared<Entity>();
		entity->Initialize(this);
		m_Entities[entity->GetGuid()] = entity;
		return entity;
	}

	std::shared_ptr<Entity> World::CreateEntity(const xg::Guid& guid)
	{
		if (m_Entities[guid]) {
			return nullptr;
		}

		auto entity = std::make_shared<Entity>(guid);
		entity->Initialize(this);
		m_Entities[guid] = entity;
		return entity;
	}

	void World::DeleteEntity(const xg::Guid& guid)
	{
		auto entity = m_Entities[guid];
		if (entity) {
			entity->Finalize();
			m_Entities.erase(guid);
		}
	}

	std::shared_ptr<Entity>	World::GetEntity(const xg::Guid& guid)
	{
		if (!m_Entities[guid]) {
			return nullptr;
		}
		return m_Entities[guid];
	}

	size_t World::GetEntityCount()
	{
		return m_Entities.size();
	}
}