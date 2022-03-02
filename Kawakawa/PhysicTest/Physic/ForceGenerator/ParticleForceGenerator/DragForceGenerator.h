#pragma once

#include "IParticleForceGenerator.h"
#include "../../Entities/Particle.h"

namespace Physic
{
	class DragForceGenerator : public IParticleForceGenerator
	{
	public:
		DragForceGenerator(float k1, float k2);
		~DragForceGenerator();
		virtual void UpdateForces(Particle* particle) override;

	private:
		float k1; // Velocity drag coefficient
		float k2; // Velocity squared drag coefficient
	};
}
