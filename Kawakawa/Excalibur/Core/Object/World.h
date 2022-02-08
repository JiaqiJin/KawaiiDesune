#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "../../Interface/IModule.h"
#include "System/MeshRenderSystem.h"
#include "System/CameraSystem.h"
#include "Entity.h"

#include <crossguid/guid.hpp>
#include "../../Platform/Application.h"

namespace Excalibur
{
	class Entity;
	class Application;

	class World : public ITickableModule
	{
	public:
		World(Application* app);
		virtual int	Initialize() noexcept;
		virtual void Finalize() noexcept;
		virtual void Tick() noexcept;
		void Render();

		std::shared_ptr<Entity> CreateEntity();
		std::shared_ptr<Entity> CreateEntity(const xg::Guid& guid);
		void DeleteEntity(const xg::Guid& guid);
		std::shared_ptr<Entity>	GetEntity(const xg::Guid& guid);
		size_t GetEntityCount();
	public:
		Application* m_App;

	private:
		std::unordered_map<xg::Guid, std::shared_ptr<Entity>> m_Entities;

		CameraSystem* m_CameraSystem;
		MeshRenderSystem* m_MeshRenderSystem;
	};
}