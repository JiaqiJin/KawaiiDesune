#include "../PrecompiledHeader.h"
#include "SpringForceGenerator.h"
#include "../../Entities/Particle.h"

namespace Physic
{
	SpringForceGenerator::SpringForceGenerator(Particle* particle, float stiffness, float restLength)
		: secondParticle(particle), springData(stiffness, restLength)
	{

	}

	SpringForceGenerator::~SpringForceGenerator()
	{

	}

	void SpringForceGenerator::UpdateForces(Particle* particle)
	{
		glm::vec3 generatedForce = particle->GetPosition() - secondParticle->GetPosition();
		float mag = glm::length(generatedForce);

		// Apply Hooke's law
		mag = springData.stiffness * abs(mag - springData.restLength);

		glm::normalize(generatedForce);
		generatedForce *= -mag;
		particle->AddForce(generatedForce);
	}
}