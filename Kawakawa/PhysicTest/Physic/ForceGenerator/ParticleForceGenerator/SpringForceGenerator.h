#pragma once

#include "IParticleForceGenerator.h"
#include "../Common.h"

namespace Physic
{
	class Particle;

	class SpringForceGenerator : public IParticleForceGenerator
	{
	public:
		SpringForceGenerator(Particle* particle, float stiffness, float restLength);
		~SpringForceGenerator();

		virtual void UpdateForces(Particle* particle);

	protected:
		Particle* secondParticle;
		SpringData springData;
	};
}