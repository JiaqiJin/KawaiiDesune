#pragma once
#include "../Entity.h"
#include "../../../Interface/ISystem.h"

namespace Excalibur
{
	class World;

	class PhysicsSystem : public ISystem
	{
	public:
		PhysicsSystem(World* world);

		virtual int	Initialize();
		virtual void Finalize();
		virtual void Tick();
	private:
		World* m_World;
	};
}