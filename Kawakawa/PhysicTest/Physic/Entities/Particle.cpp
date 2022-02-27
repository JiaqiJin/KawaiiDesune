#include "../PrecompiledHeader.h"
#include "Particle.h"

namespace Physic
{
	Particle::Particle(const glm::vec3& position, const float inverseMass)
	{

	}

	Particle::~Particle()
	{

	}

	void Particle::Integrate(float deltaTime)
	{
		// We don¡¯t integrate things with infinite mass.
		if (inverseMass <= 0.0f) 
			return;

		assert(deltaTime > 0.0f);

		// Update acceleration using newton's second law (f = m * a)
		acceleration = accumForces * inverseMass;

		//velocity = velocity * pow(damping, GameTime::deltaTime) + acceleration * GameTime::deltaTime;
		velocity = velocity * linearDamping + acceleration * deltaTime;

		ClearAccumulators();
	}

	// Clear all the accumulators
	void Particle::ClearAccumulators()
	{
		acceleration.x = 0;
		acceleration.y = 0;
		acceleration.z = 0;
	}
}