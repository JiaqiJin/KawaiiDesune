#include "../PrecompiledHeader.h"
#include "DragForceGenerator.h"

namespace Physic
{
	DragForceGenerator::DragForceGenerator(float k1_, float k2_)
		: k1(k1_), k2(k2_)
	{

	}

	DragForceGenerator::~DragForceGenerator()
	{

	}

	void DragForceGenerator::UpdateForces(Particle* particle)
	{
		// Check for infinite mass
		if (particle->GetIsInfiniteMass())
			return;

		// Calculate the total drag coefficient.
		glm::vec3& velocity = particle->GetVelocity();
		float dragScalar = glm::length(particle->GetVelocity());
		dragScalar = k1 * dragScalar + k2 * dragScalar * dragScalar;

		// Calculate the final force and apply it
		glm::vec3 dragForce = glm::normalize(velocity); // Get the direction vector of the velocity so that drag is against it
		dragForce *= -dragScalar;

		particle->AddForce(dragForce);
	}
}
