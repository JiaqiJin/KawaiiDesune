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

		// Virtual Functions
		virtual int		Initialize() noexcept;
		virtual void	Finalize() noexcept;
		virtual void	Tick() noexcept;
		virtual void	Render() noexcept;
	private:
		std::unordered_map<xg::Guid, Entity> m_Entities;

		// System
	};
}