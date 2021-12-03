#include "World.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

namespace Excalibur
{
	World::World()
	{

	}

	std::shared_ptr<Entity>	World::CreateEntity()
	{
		auto entity = std::make_shared<Entity>();
		entity->Initialize(this);
		m_Entities[entity->GetGuid()] = entity;
		return entity;
	}

	std::shared_ptr<Entity>	World::CreateEntity(const xg::Guid& guid)
	{
		if (m_Entities[guid])
		{
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
		if (entity)
		{
			entity->Finalize();
			m_Entities.erase(guid);
		}
	}

	std::shared_ptr<Entity>	World::GetEntity(const xg::Guid& guid)
	{
		if (!m_Entities[guid]) 
		{
			return nullptr;
		}
		return m_Entities[guid];
	}

	size_t World::GetEntityCount()
	{
		return m_Entities.size();
	}

	void World::LoadScene(const std::string& scenePath)
	{
		Assimp::Importer importer2;
		const aiScene* scene = importer2.ReadFile(scenePath,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);

		assert(scene);


		for (unsigned int i = 0; i < scene->mRootNode->mNumChildren; ++i)
		{
			auto child = scene->mRootNode->mChildren[i];
			if (child->mNumMeshes <= 0) 
			{
				continue;
			}

			auto entity = CreateEntity();
			auto comp = entity->AddComponent<MeshRenderComponent>();
			for (unsigned int j = 0; j < child->mNumMeshes; ++j)
			{
				auto midx = child->mMeshes[j];
				auto mesh = scene->mMeshes[midx];
				//auto robj = comp->
			}
		}
	}

	// ---------------------- ------------------------------
	int	World::Initialize() noexcept
	{
		return 1;
	}

	void World::Tick() noexcept
	{

	}

	void World::Finalize() noexcept
	{
		m_Entities.clear();
	}

	void World::Render() noexcept
	{

	}

}