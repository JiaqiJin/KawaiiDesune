#include "../PrecompiledHeader.h"
#include "AnchoredSpringForceGenerator.h"
#include "../../Entities/Particle.h"

namespace Physic
{
	AnchoredSpringForceGenerator::AnchoredSpringForceGenerator(glm::vec3* anchorP, float stiffness, float restLength)
		: springData(stiffness, restLength), anchorPoint(anchorP)
	{

	}

	AnchoredSpringForceGenerator::~AnchoredSpringForceGenerator()
	{

	}

	void AnchoredSpringForceGenerator::UpdateForces(Particle* particle)
	{
		glm::vec3 generatedForce = particle->GetPosition() - *anchorPoint;
		float mag = glm::length(generatedForce);

		// Apply Hooke's law
		mag = springData.stiffness * abs(mag - springData.restLength);

		glm::normalize(generatedForce);
		generatedForce *= -mag;
		particle->AddForce(generatedForce);
	}
}