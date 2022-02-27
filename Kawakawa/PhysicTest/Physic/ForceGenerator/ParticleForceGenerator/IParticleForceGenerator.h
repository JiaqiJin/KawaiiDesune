#pragma once

namespace Physic
{
	class Particle;

	class IParticleForceGenerator
	{
	public:
		virtual void UpdateForces(Particle* particle) = 0;
	};
}