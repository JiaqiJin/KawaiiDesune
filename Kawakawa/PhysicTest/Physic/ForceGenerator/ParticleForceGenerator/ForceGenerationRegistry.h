#pragma once

#include "../../Entities/Particle.h"
#include "IParticleForceGenerator.h"

namespace Physic
{
	// Register which force generators affect which particles.
	class ForceGenerationRegistry
	{
	public:
		ForceGenerationRegistry() {}
		~ForceGenerationRegistry() {}

		// Add a force generator to a particle's registry
		inline void AddForceGenerator(IParticleForceGenerator* forceGenerator) { forceGenerators.push_back(forceGenerator); }
		// Remove a force generator from a particle's registry
		inline void RemoveForceGenerator(const int index) { forceGenerators.erase(forceGenerators.begin() + index); }
		// Remove by force generator
		void RemoveForceGenerator(const IParticleForceGenerator* forceGenerator); // Remove a specific force generator
		// Removes All the force generators that we have
		inline void clearForceGenerators() { forceGenerators.clear(); }
		// Update the force generators
		void UpdateForceGenerators(Particle* particle); // Update the force generators using a particle
	private:
		// Holds the list of registrations.
		std::vector<IParticleForceGenerator*> forceGenerators;
	};
}