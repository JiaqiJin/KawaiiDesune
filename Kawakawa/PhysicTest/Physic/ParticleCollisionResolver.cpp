#include "../PrecompiledHeader.h"
#include "ParticleCollisionResolver.h"

namespace Physic
{
	ParticleCollisionResolver::ParticleCollisionResolver(unsigned int numIter) 
		: numIterations(numIter)
	{
	}

	ParticleCollisionResolver::~ParticleCollisionResolver()
	{
	}

	void ParticleCollisionResolver::ResolveContacts(std::list<ParticleCollision>& particleCollisions)
	{
		currentIterations = 0;
		while (currentIterations != numIterations)
		{
			float maxSeparatingVelocity = 0;
			auto maxCollisionIter = particleCollisions.begin();
			for (auto iter = particleCollisions.begin(); iter != particleCollisions.end(); ++iter)
			{
				float sepVelocity = iter->CalculateSeparatingVelocity();
				if (sepVelocity < maxSeparatingVelocity)
				{
					maxSeparatingVelocity = sepVelocity;
					maxCollisionIter = iter;
				}
			}

			// Resolver the collision 
			maxCollisionIter->Resolve();
			++currentIterations;
		}
	}
}