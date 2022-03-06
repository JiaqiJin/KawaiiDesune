#pragma once

#include "Entities/Particle.h"

/*
* A contact represents two objects in contact
* Resolving a contact removes their interpenetration, and applies sufficient
* impulse to keep them apart. Colliding bodies may also rebound.
*/

namespace Physic
{
	class ParticleCollision
	{
	public:

	public:
		ParticleCollision(Particle* particle1, Particle* particle2, float restitution, glm::vec3& normal);
		~ParticleCollision();

		Particle* particles[2]; // Holds the particles that are involved in the Colliding.
		float restitution;  // Restitution constance that allows us to know the ratio of how much separating velocity we will use
		float penetration; // The depth of penetration between the 2 objects
		glm::vec3 collisionNormal; // Holds the direction of the collision in world coordinates.

		void Resolve(); // Resolve a collision
		float CalculateSeparatingVelocity() const; // Calculates the separating velocity at this contact.
		void SetCollisionNormal(glm::vec3& normal) { collisionNormal = normal; }
	private:
		void ResolveVelocity(); // Handles the impulse calculations for this collision.
		void ResolveInterpenetration(); // Resolves collision penetration
	};
}