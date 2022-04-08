#pragma once

#include "Body.h"

namespace Physic
{
	// A rigid body is the basic simulation object in the physics core
	class RigidBody : public Body
	{
	public:
		RigidBody();
		~RigidBody();

		// Update the values of the rigidbody
		void Integrate(float deltaTime) override;

		virtual void ClearAccumulators() override;

		// Calculate the transformation matrix
		void CalculateObjToWorldMat();

		// Calculate intrinsic rigidbody data
		void CalculateBodyData();
	private:
		float angularDamping; // Between 0-1
		glm::vec3 rotation;
		// converting body space into world spaceand vice versa.
		glm::mat3x4 objToWorldMat;
		glm::vec3 accumTorque; // Accumulated torque
	};
}