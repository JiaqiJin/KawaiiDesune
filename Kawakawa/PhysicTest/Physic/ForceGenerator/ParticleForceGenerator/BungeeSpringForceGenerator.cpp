#include "../PrecompiledHeader.h"
#include "BungeeSpringForceGenerator.h"
#include "../../Entities/Particle.h"

namespace Physic
{
	BungeeSpringForceGenerator::BungeeSpringForceGenerator(glm::vec3* anchorP, float stiffness, float restLength)
		: AnchoredSpringForceGenerator(anchorP, stiffness, restLength)
	{

	}

	BungeeSpringForceGenerator::~BungeeSpringForceGenerator()
	{

	}

	void BungeeSpringForceGenerator::UpdateForces(Particle* particle)
	{
		glm::vec3 generatedForce = particle->GetPosition() - *anchorPoint;
		float mag = glm::length(generatedForce);

		// Check that the spring is extended since no force will be generated unless it is
		if (mag <= springData.restLength)
			return;

		// Apply Hooke's law
		mag = springData.stiffness * abs(mag - springData.restLength);

		glm::normalize(generatedForce);
		generatedForce *= -mag;
		particle->AddForce(generatedForce);
	}
}