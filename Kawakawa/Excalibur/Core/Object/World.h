#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "../../Interface/IModule.h"

#include "Entity.h"
#include "System/MeshRenderSystem.h"
#include "System/CameraSystem.h"

#include <crossguid/guid.hpp>

namespace Excalibur
{
	class Entity;
	class Application;

	class World : public ITickableModule
	{
	public:
		virtual int	Initialize();
		virtual void Finalize();
		virtual void Tick();
		virtual void Render();
		
	public:
		World(Application* app);

		std::shared_ptr<Entity>	CreateEntity();
		std::shared_ptr<Entity>	CreateEntity(const xg::Guid& guid);
		void DeleteEntity(const xg::Guid& guid);
		std::shared_ptr<Entity>	GetEntity(const xg::Guid& guid);
		size_t GetEntityCount();

		void LoadScene(const std::string& scenePath);
		void DumpEntities();


	public:
		MeshRenderSystem* GetMeshRenderSystem() { return m_MeshRenderSystem; }
		CameraSystem* GetCameraSystem() { return m_CameraSystem; }

	public:
		Application* mApp;

	private:
		std::unordered_map<xg::Guid, std::shared_ptr<Entity>> m_Entities;

		// systems
		MeshRenderSystem*	m_MeshRenderSystem;
		CameraSystem* m_CameraSystem;
	};

}