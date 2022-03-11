#pragma once

#include "Entities/Particle.h"
#include "ParticleCollision.h"
#include "ParticleCollisionGenerator.h"

namespace Physic
{
	// Links connect two particles together, generating a contact if
	// they violate the constraints of their link.
	class ParticleLink : public ParticleCollisionGenerator
	{
	public:
		ParticleLink();
		~ParticleLink();

		Particle* particles[2]; // Holds the pair of particles that are connected by this link.

		inline float GetCurrentLengthSqr() const { return glm::length2(particles[0]->GetPosition() - particles[1]->GetPosition()); }
		virtual void GenerateContact(ParticleCollision* collisions, unsigned int limit) = 0;
	};

	/**
	* Cables link a pair of particles, generating a contact if they
	* stray too far apart.
	*/
	class ParticleCable : public ParticleLink
	{
	public:
		ParticleCable(float maxLengthSqr, float restitution);
		~ParticleCable();

		// Holds the maximum length of the cable.
		float maxLengthSqr;

		// Holds the restitution (bounciness) of the cable.
		float restitution;

		virtual void GenerateContact(ParticleCollision* collisions, unsigned int limit);
	};

	// Rods link a pair of particles, generating a contact if they
	// stray too far apart or too close.
	class ParticleRod : public ParticleLink
	{
	public:
		ParticleRod(float lengthSqr);
		~ParticleRod();

		float lengthSqr; // Length of the rod

		virtual void GenerateContact(ParticleCollision* collisions, unsigned int limit);
	};
}