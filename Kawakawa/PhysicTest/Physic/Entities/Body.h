#pragma once

#include "IIntegratable.h"

#define MAX_MASS 10000000.f

namespace Physic
{
	class Body : IIntegratable
	{
	public:
		Body();
		~Body();

		// Add force 
		inline void AddForce(const glm::vec3& force) { accumForces += force; }
		// Implementation of the physics engine update
		virtual void Integrate(float deltaTime) = 0;
		// Clear all the accumulators
		virtual void ClearAccumulators() = 0;

	protected:
		glm::vec3 velocity; // Holds the linear velocity of the particle
		glm::vec3 acceleration; // Holds the acceleration of the particle
		glm::vec3 position; // Holds the linear position of the particle
		glm::vec3 accumForces; // Accumulated forces 
		float linearDamping; // Simple damping for frictional forces (between 0 and 1)
		float inverseMass; // The mass of the particle (it is more useful to have objects with infinite mass(immovable) than zero mass)
	};
}