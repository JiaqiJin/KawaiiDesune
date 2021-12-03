#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "../../Interface/IModule.h"
#include "System/MeshRenderSystem.h"
#include "Entity.h"

#include <crossguid/guid.hpp>

namespace Excalibur
{
	class Entity;

	class Entity;
	class World : public ITickableModule 
	{
	public:
		virtual int		Initialize() noexcept;
		virtual void	Finalize() noexcept;
		virtual void	Tick() noexcept;
		virtual void	Render() noexcept;

	public:
		World();

		std::shared_ptr<Entity>	CreateEntity();
		std::shared_ptr<Entity>	CreateEntity(const Guid& guid);
		void	DeleteEntity(const Guid& guid);
		std::shared_ptr<Entity>	GetEntity(const Guid& guid);
		size_t GetEntityCount();

		void LoadScene(const std::string& scenePath);
		void DumpEntities();

		MeshRenderSystem* GetMeshRenderSystem() { return m_MeshRenderSystem; }

	private:
		std::unordered_map<Guid, std::shared_ptr<Entity>> m_Entities;

		// systems
		MeshRenderSystem* m_MeshRenderSystem;
	};

}