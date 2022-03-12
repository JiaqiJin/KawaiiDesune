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

	void ParticleCable::GenerateContact(std::list<ParticleCollision>& collisions, unsigned int limit)
	{
		// If the cable is not taut enough no contact will be generated
		float lengthSqr = GetCurrentLengthSqr();
		if (lengthSqr < maxLengthSqr)
			return;

		// Calculate collision normal
		glm::vec3 collisionNormal = glm::normalize(particles[1]->GetPosition() - particles[0]->GetPosition());

		// Create a new particle collision with these 2 particles
		ParticleCollision collision = ParticleCollision(particles[0], particles[1], restitution, collisionNormal);
		collision.penetration = sqrt(lengthSqr - maxLengthSqr); 

		collisions.push_back(collision);
	}

	// --------------------- ParticleRod -----------------------------
	ParticleRod::ParticleRod(float lengthSqr) : lengthSqr(lengthSqr)
	{
	}

	ParticleRod::~ParticleRod()
	{
	}

	void ParticleRod::GenerateContact(std::list<ParticleCollision>& collisions, unsigned int limit)
	{
		// Check the length of the rod
		float currentLengthSqr = GetCurrentLengthSqr();
		if (currentLengthSqr == lengthSqr)
			return;

		// Calculate the collision normal
		glm::vec3 normal = glm::normalize(particles[1]->GetPosition() - particles[0]->GetPosition());

		// Check which direction we need to generate the particle contact collision
		if (currentLengthSqr < lengthSqr)
			normal = -normal;

		ParticleCollision collision = ParticleCollision(particles[0], particles[1], 0, normal);
		collision.penetration = sqrt(abs(currentLengthSqr - lengthSqr));

		collisions.push_back(collision);
	}
}