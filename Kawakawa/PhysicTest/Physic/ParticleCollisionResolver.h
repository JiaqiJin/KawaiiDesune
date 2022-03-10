#pragma once

#include "ParticleCollision.h"

namespace Physic
{
	class ParticleCollisionResolver
	{
	public:
		ParticleCollisionResolver(unsigned int numIterations);
		~ParticleCollisionResolver();

		inline void SetIterations(unsigned int iterations) { numIterations = iterations; }
		void ResolveContacts(std::list<ParticleCollision>& particleCollisions);

	private:
		unsigned int numIterations; // Holds the number of iterations allowed.
		unsigned int currentIterations; // The number of iterations that we have currently used
	};
}