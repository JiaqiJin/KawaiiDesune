#pragma once

#include "IIntegratable.h"

#define MAX_MASS 10000000.f

namespace Physic
{
	class Body : IIntegratable
	{
	public:
		Body();
		~Body();

		// Add force 
		inline void AddForce(const glm::vec3& force) { accumForces += force; }
		// Implementation of the physics engine update
		virtual void Integrate(float deltaTime) = 0;
		// Clear all the accumulators
		virtual void ClearAccumulators() = 0;

		// Setters 
		inline void SetInverseMass(const float inverseMass) { this->inverseMass = inverseMass; }
		inline void SetVelocity(const glm::vec3& velocity) { this->velocity = velocity; }
		inline void SetAcceleration(const glm::vec3& acceleration) { this->acceleration = acceleration; }
		inline void SetLinearDamping(const float damping) { this->linearDamping = damping; }
		void SetMass(const float mass);

		// Getters
		inline bool GetIsInfiniteMass() { return inverseMass == 0.f; }
		inline float GetInverseMass() const { return inverseMass; }
		inline float GetDamping() const { return linearDamping; }
		inline glm::vec3& GetVelocity() { return velocity; }
		inline glm::vec3& GetAcceleration() { return acceleration; }
		inline glm::vec3& GetAccumForces() { return accumForces; }
		inline glm::vec3& GetPosition() { return position; }
		float GetMass() const;

	protected:
		glm::vec3 velocity; // Holds the linear velocity of the particle
		glm::vec3 acceleration; // Holds the acceleration of the particle
		glm::vec3 position; // Holds the linear position of the particle
		glm::vec3 accumForces; // Accumulated forces 
		float linearDamping; // Simple damping for frictional forces (between 0 and 1)
		float inverseMass; // The mass of the particle (it is more useful to have objects with infinite mass(immovable) than zero mass)
	};
}