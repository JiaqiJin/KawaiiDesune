#include "PhysicsSystem.h"
#include "../World.h"

namespace Excalibur
{
	PhysicsSystem::PhysicsSystem(World* world)
		: m_World(world)
	{

	}

	int	PhysicsSystem::Initialize()
	{
		return 0;
	}

	void PhysicsSystem::Finalize()
	{

	}

	void PhysicsSystem::Tick()
	{

	}
}