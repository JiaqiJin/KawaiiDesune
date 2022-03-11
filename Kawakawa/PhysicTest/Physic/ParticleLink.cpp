#include "../PrecompiledHeader.h"+
#include "ParticleLink.h"

namespace Physic
{
	ParticleLink::ParticleLink()
	{

	}

	ParticleLink::~ParticleLink()
	{

	}

	// --------------------- ParticleCable -----------------------------
	ParticleCable::ParticleCable(float maxLengthSqr, float restitution)
		: maxLengthSqr(maxLengthSqr), restitution(restitution)
	{

	}

	ParticleCable::~ParticleCable()
	{

	}

	void ParticleCable::GenerateContact(ParticleCollision* collisions, unsigned int limit)
	{
		float realLengthSqr = GetCurrentLengthSqr();

		// Check if we¡¯re overextended.
		if (realLengthSqr < maxLengthSqr)
			return;

		// Otherwise, return the contact.
		collisions->particles[0] = particles[0];
		collisions->particles[1] = particles[1];

		// Calculate the normal.
		glm::vec3 collisionNormal = glm::normalize(particles[1]->GetPosition() - particles[0]->GetPosition());
		collisions->collisionNormal = collisionNormal;

		collisions->penetration = sqrt(realLengthSqr - maxLengthSqr);
		collisions->restitution = restitution;
	}

	// --------------------- ParticleRod -----------------------------
	ParticleRod::ParticleRod(float lengthSqr) : lengthSqr(lengthSqr)
	{
	}

	ParticleRod::~ParticleRod()
	{
	}

	void ParticleRod::GenerateContact(ParticleCollision* collisions, unsigned int limit)
	{
		float currentLengthSqr = GetCurrentLengthSqr();
		if (currentLengthSqr == lengthSqr)
			return;

		// Otherwise, return the contact.
		collisions->particles[0] = particles[0];
		collisions->particles[1] = particles[1];

		// Calculate the normal.
		glm::vec3 normal = glm::normalize(particles[1]->GetPosition() - particles[0]->GetPosition());
		// The contact normal depends on whether we¡¯re extending or compressing.
			if (currentLengthSqr > lengthSqr) {
				collisions->collisionNormal = normal;
				collisions->penetration = sqrt(currentLengthSqr - lengthSqr);
			}
			else {
				collisions->collisionNormal = -normal;
				collisions->penetration = sqrt(lengthSqr - currentLengthSqr);
			}

		// Always use zero restitution (no bounciness).
		collisions->restitution = 0;
	}
}