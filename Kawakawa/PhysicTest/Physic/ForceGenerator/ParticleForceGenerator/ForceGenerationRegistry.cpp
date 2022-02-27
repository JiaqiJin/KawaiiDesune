#include "../PrecompiledHeader.h"
#include "ForceGenerationRegistry.h"

namespace Physic
{
	void ForceGenerationRegistry::RemoveForceGenerator(const IParticleForceGenerator* forceGenerator)
	{
		for (auto iter = forceGenerators.begin(); iter != forceGenerators.end(); ++iter)
		{
			if (*iter == forceGenerator)
			{
				forceGenerators.erase(iter);
				break;
			}
		}
	}

	void ForceGenerationRegistry::UpdateForceGenerators(Particle* particle)
	{
		for (auto iter = forceGenerators.begin(); iter != forceGenerators.end(); ++iter)
		{
			(*iter)->UpdateForces(particle);
		}
	}
}