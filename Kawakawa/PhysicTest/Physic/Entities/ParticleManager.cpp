#include "../PrecompiledHeader.h"
#include "ParticleManager.h"

namespace Physic
{
	ParticleManager::ParticleManager()
		: collisionResolver(1)
	{

	}

	ParticleManager::ParticleManager(unsigned int maxContacts, unsigned int numIterations)
		: maxContacts(maxContacts), numIterations(numIterations), collisionResolver(1)
	{

	}

	ParticleManager::~ParticleManager()
	{

	}

	void ParticleManager::PhysicsUpdate()
	{
		// Update the force generators
		UpdateForces();
		// Update the particles according to the accumulated forces
		UpdateParticles();
		// Update the positions according to the collisions generated
		GenerateCollisions();

		// Resolve collisions (THIS MIGHT BE WRONG) makes some sense but not sure 
		collisionResolver.SetIterations((unsigned int)particleCollisions.size() * 2);
		collisionResolver.ResolveContacts(particleCollisions);
	}

	unsigned int ParticleManager::GenerateCollisions()
	{
		for (auto iter = particleCollisionGenerators.begin(); iter != particleCollisionGenerators.end(); ++iter)
		{
			(*iter)->GenerateContact(particleCollisions, maxContacts);
		}

		return 1;
	}

	void ParticleManager::UpdateParticles()
	{

	}

	void ParticleManager::UpdateForces()
	{
		for (auto iter = forceGenerationRegistry.begin(); iter != forceGenerationRegistry.end(); ++iter)
		{
			// Update force generators with their representative particle
			iter->registry->UpdateForceGenerators(iter->particle);
		}
	}
}