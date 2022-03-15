#include "../PrecompiledHeader.h"
#include "Rigidbody.h"

namespace Physic
{
	RigidBody::RigidBody()
	{

	}

	RigidBody::~RigidBody()
	{

	}

	void RigidBody::Integrate(float deltaTime)
	{

	}

	void RigidBody::ClearAccumulators()
	{
		accumForces.x = 0;
		accumForces.y = 0;
		accumForces.z = 0;
	}

	void RigidBody::CalculateBodyData()
	{
		// Normalize orientation
		transform.SetOrientation(glm::normalize(transform.GetOrientation()));

		// Calculate the object to world transform
		CalculateObjToWorldMat();
	}

	void RigidBody::CalculateObjToWorldMat()
	{
		glm::quat& orientation = transform.GetOrientation();
		glm::vec3& position = transform.GetPosition();

		// i,j,k -> x,y,z
		objToWorldMat[0][0] = 1 - 2 * orientation.y * orientation.y - 2 * orientation.z * orientation.z;
		objToWorldMat[0][1] = 2 * orientation.x * orientation.y - 2 * orientation.w * orientation.z;
		objToWorldMat[0][2] = 2 * orientation.x * orientation.z + 2 * orientation.w * orientation.y;
		objToWorldMat[0][3] = position.x;
		objToWorldMat[1][0] = 2 * orientation.x * orientation.y + 2 * orientation.w * orientation.z;
		objToWorldMat[1][1] = 1 - 2 * orientation.x * orientation.x - 2 * orientation.z * orientation.z;
		objToWorldMat[1][2] = 2 * orientation.y * orientation.z - 2 * orientation.w * orientation.x;
		objToWorldMat[1][3] = position.y;
		objToWorldMat[2][0] = 2 * orientation.x * orientation.z - 2 * orientation.w * orientation.y;
		objToWorldMat[2][1] = 2 * orientation.y * orientation.z + 2 * orientation.w * orientation.x;
		objToWorldMat[2][2] = 1 - 2 * orientation.x * orientation.x - 2 * orientation.y * orientation.y;
		objToWorldMat[2][3] = position.z;
	}
}