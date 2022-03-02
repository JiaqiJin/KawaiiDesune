#pragma once

#include "IParticleForceGenerator.h"
#include "../Common.h"

namespace Physic
{
	class Particle;

	class BuoyancyForceGenerator : public IParticleForceGenerator
	{
	public:
		BuoyancyForceGenerator(float density, float volume, float WaterDepth, float maxD);
		~BuoyancyForceGenerator();

		virtual void UpdateForces(Particle* particle) override;
	private:

		// The maximum submersion depth of the object before it generates its maximum buoyancy force.
		float maxDepth;
		//The volume of the object.
		float volume;
		// The height of the water plane above y = 0.
		float waterHeight;
		// The density of the liquid. Pure water has a density of 1000 kg per cubic meter.
		float liquidDensity;
	};
}