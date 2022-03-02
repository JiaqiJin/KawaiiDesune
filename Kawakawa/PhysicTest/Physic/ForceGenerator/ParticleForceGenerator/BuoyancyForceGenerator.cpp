#include "../PrecompiledHeader.h"
#include "BuoyancyForceGenerator.h"
#include "../../Entities/Particle.h"

namespace Physic
{
	BuoyancyForceGenerator::BuoyancyForceGenerator(float density, float v, float wh, float maxD)
		: liquidDensity(density), waterHeight(wh), volume(v), maxDepth(maxD)
	{

	}

	BuoyancyForceGenerator::~BuoyancyForceGenerator()
	{

	}

	void BuoyancyForceGenerator::UpdateForces(Particle* particle)
	{
		float currentDepth = particle->GetPosition().y;

		// Check if we¡¯re out of the water.
		if (currentDepth >= maxDepth + waterHeight)
			return;

		// Apply some buoyancy force
		glm::vec3 force;

		// Check if we¡¯re at maximum depth.
		if (currentDepth <= waterHeight - maxDepth)
		{
			force.y = volume * liquidDensity;
		}
		else
		{
			force.y = liquidDensity * volume *
				(currentDepth - maxDepth - waterHeight) / 2 * maxDepth;
		}
		// Otherwise we are partly submerged.

		particle->AddForce(force);
	}
}