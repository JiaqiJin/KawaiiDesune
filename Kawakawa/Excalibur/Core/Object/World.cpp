#include "World.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

namespace Excalibur
{
	World::World()
		: mMeshRenderSystem(nullptr)
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

	void World::DumpEntities()
	{
		cout << "dump entities:" << endl;
		for (auto pair : m_Entities) {
			auto guid = pair.first;
			auto entity = pair.second;

			cout << "guid: " << guid << endl;
			cout << "transform component:" << endl;
			auto position = entity->GetComponent<TransformComponent>()->GetPosition();
			cout << "position: " << "(" << position.x << "," << position.y << "," << position.z << "," << endl;

			auto meshRender = entity->GetComponent<MeshRenderComponent>();
			if (meshRender) {
				cout << "MeshRenderComponent:" << endl;
				for (int i = 0; i < meshRender->GetRenderObjectCount(); ++i) {
					auto robj = meshRender->GetRenderObject(i);
					cout << "RenderObject: " << robj->GetName() << endl;
				}
			}
			cout << endl;
		}
	}

	// ---------------------- ------------------------------
	int	World::Initialize() noexcept
	{
		mMeshRenderSystem = new MeshRenderSystem();
		mMeshRenderSystem->Initialize();

		return 0;
	}

	void World::Tick() noexcept
	{
		mMeshRenderSystem->Tick();
	}

	void World::Finalize() noexcept
	{
		m_Entities.clear();
	}

	void World::Render() noexcept
	{
		mMeshRenderSystem->Render();
	}

}