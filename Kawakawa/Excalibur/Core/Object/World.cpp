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
		m_MeshRenderSystem->Tick();
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

	void World::LoadScene(const std::string& scenePath)
	{
		Assimp::Importer importer2;
		const aiScene* scene = importer2.ReadFile(scenePath,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);
		assert(scene);

		// Load all mesh
		for (unsigned int j = 0; j < scene->mNumMeshes; ++j)
		{
			auto mesh = scene->mMeshes[j];
			m_MeshRenderSystem->LoadMesh(mesh, scene);
		}

		// build main camera
		auto camera = CreateEntity();
		camera->AddComponent<CameraComponent>();
		m_CameraSystem->SetMainCamera(camera);

		// build scene graph entity
		for (unsigned int i = 0; i < scene->mRootNode->mNumChildren; i++)
		{
			auto child = scene->mRootNode->mChildren[i];
			if (child->mNumMeshes <= 0)
				continue;

			auto entity = CreateEntity();

			aiVector3D scaling, rotation, position;
			child->mTransformation.Decompose(scaling, rotation, position);
			auto transformation = entity->GetComponent<TransformComponent>();
			transformation->SetPosition(Vector3f(position.x, position.y, position.z));
			transformation->SetRotation(Vector3f(rotation.x, rotation.y, rotation.z));
			transformation->SetScale(Vector3f(scaling.x, scaling.y, scaling.z));

			auto comp = entity->AddComponent<MeshRenderComponent>();
			for (unsigned int j = 0; j < child->mNumMeshes; ++j) 
			{
				auto midx = child->mMeshes[j];
				comp->m_MeshIndex.push_back(midx);
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
			cout << "position: " << "(" << position.x() << "," << position.y() << "," << position.z() << ")" << endl;
		}
	}
}