#pragma once
#include "IParticleForceGenerator.h"
#include "../Common.h"

namespace Physic
{
	class AnchoredSpringForceGenerator : public IParticleForceGenerator
	{
	public:
		AnchoredSpringForceGenerator(glm::vec3* anchorPoint, float stiffness, float restLength);
		~AnchoredSpringForceGenerator();

		virtual void UpdateForces(Particle* particle) override;

		inline void SetAnchorPoint(glm::vec3* point) { anchorPoint = point; }
	protected:
		SpringData springData;
		// The location of the anchored end of the spring
		glm::vec3* anchorPoint;
	};
}