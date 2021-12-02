#pragma once

#include "IModule.h"

namespace Excalibur
{
	class Entity;

	class IComponent : public IModule
	{
	public:
		IComponent() : m_Master(nullptr), m_IsActive(true) {}

		virtual int Initialize() noexcept = 0;
		virtual void Finalize() noexcept = 0;


		bool	IsActive() { return m_IsActive; }
		void	SetActive(bool active) { m_IsActive = active; }

		Entity* GetMaster() { return m_Master; }
		void	SetMaster(Entity* master) { m_Master = master; }

	protected:
		Entity* m_Master;
		bool m_IsActive;
	};
}