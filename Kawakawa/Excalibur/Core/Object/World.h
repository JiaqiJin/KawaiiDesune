#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "../../Interface/IModule.h"
#include "Entity.h"

#include <crossguid/guid.hpp>

namespace Excalibur
{
	class Entity;

	class World : public ITickableModule
	{
	public:
		World();

		// Virtual Functions
		virtual int		Initialize() noexcept;
		virtual void	Finalize() noexcept;
		virtual void	Tick() noexcept;
		virtual void	Render() noexcept;

		std::shared_ptr<Entity>	CreateEntity();
		std::shared_ptr<Entity>	CreateEntity(const xg::Guid& guid);
		void DeleteEntity(const xg::Guid& guid);
		std::shared_ptr<Entity>	GetEntity(const xg::Guid& guid);
		size_t GetEntityCount();

		void LoadScene(const std::string& scenePath);
	private:
		std::unordered_map<xg::Guid, std::shared_ptr<Entity>> m_Entities;

		// System
	};
}