#include "../PrecompiledHeader.h"
#include "ParticleCollision.h"
#include "../Utils/GameTime.h"

namespace Physic
{
	ParticleCollision::ParticleCollision(Particle* particle1, Particle* particle2, float restitution, glm::vec3& normal)
		: particles{ particle1, particle2 }, restitution(restitution), collisionNormal(normal)
	{

	}

	ParticleCollision::~ParticleCollision()
	{

	}

	void ParticleCollision::Resolve() 
	{
		ResolveVelocity();
		ResolveInterpenetration();
	}

	float ParticleCollision::CalculateSeparatingVelocity() const
	{
		glm::vec3 relVelocity = particles[0]->GetVelocity();
		relVelocity -= particles[1]->GetVelocity();
		return glm::dot(relVelocity ,collisionNormal);
	}

	void ParticleCollision::ResolveVelocity()
	{
		float separatingVlocity = CalculateSeparatingVelocity();

		// Check if it needs to be resolved.
		if (separatingVlocity > 0)
			return;

		// Check if both have infinite mass if they do no impulse generated
		float totalInverseMass = particles[0]->GetInverseMass() + particles[1]->GetInverseMass();
		if (totalInverseMass <= 0)
			return;

		// Calculate the new separating velocity and factor in the restitution
		float newSepVelocity = -separatingVlocity * restitution;

		// Check for velocity caused by acceleration in the direction of the closing velocity
		glm::vec3 accelerationBuildUp = particles[0]->GetAcceleration() - particles[1]->GetAcceleration();
		float accelerationCausedSepVelocity = glm::dot(accelerationBuildUp, collisionNormal * GameTime::deltaTime);
		if (accelerationCausedSepVelocity < 0)
		{
			// Subtract the acceleration in the direction of the collision normal from the separating velocity
			newSepVelocity += accelerationCausedSepVelocity * restitution;

			// We don't want a negative new separating velocity
			if (newSepVelocity < 0)
				newSepVelocity = 0;
		}

		float deltaVelocity = newSepVelocity - separatingVlocity;
		// Calculate the impulse to apply.
		float impulse = deltaVelocity / totalInverseMass;

		// Find the amount of impulse per unit of inverse mass.
		glm::vec3 impulsePerMass = collisionNormal * impulse;

		//Apply Impulse
		if (!particles[0]->GetIsInfiniteMass())
			particles[0]->GetVelocity() += impulsePerMass * particles[0]->GetInverseMass();

		// Apply opposite impulse
		if (!particles[1]->GetIsInfiniteMass())
			particles[1]->GetVelocity() += impulsePerMass * -particles[1]->GetInverseMass();
	}

	void ParticleCollision::ResolveInterpenetration()
	{

		// If there is no proper penetration
		if (penetration <= 0)
			return;

		// Check that both particles are not infinite mass
		float totalInverseMass = particles[0]->GetInverseMass() + particles[1]->GetInverseMass();
		if (totalInverseMass <= 0)
			return;

		// Find the movement vector according to penetration and inverse mass
		glm::vec3 moveVector = collisionNormal * (penetration / totalInverseMass);

		// Calculate the movement amounts.
		if (particles[0]->GetInverseMass())
			particles[0]->GetPosition() += moveVector * particles[0]->GetInverseMass();

		if (particles[1]->GetInverseMass())
			particles[1]->GetPosition() += moveVector * -particles[1]->GetInverseMass();

	}
}