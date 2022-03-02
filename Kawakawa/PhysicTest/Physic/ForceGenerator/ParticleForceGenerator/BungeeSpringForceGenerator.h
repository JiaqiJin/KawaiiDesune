#pragma once

#include "AnchoredSpringForceGenerator.h"
#include "../Common.h"

namespace Physic
{
	class BungeeSpringForceGenerator : public AnchoredSpringForceGenerator
	{
	public:
		BungeeSpringForceGenerator(glm::vec3* anchorPoint, float stiffness, float restLength);
		~BungeeSpringForceGenerator();

		virtual void UpdateForces(Particle* particle) override;
	};
}