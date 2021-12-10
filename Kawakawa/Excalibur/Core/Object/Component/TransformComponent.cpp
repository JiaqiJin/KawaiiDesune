#include "TransformComponent.h"

namespace Excalibur
{
	TransformComponent::TransformComponent() :
		m_Position(0.0f, 0.0f, 0.0f),
		m_Rotation(0.0f, 0.0f, 0.0f),
		m_Scale(1.0f, 1.0f, 1.0f),
		m_WorldMatrixDirty(true)
	{
	
	}

	int TransformComponent::Initialize() noexcept
	{
		return 0;
	}


	void TransformComponent::Finalize() noexcept
	{

	}

	Matrix4f TransformComponent::GetWorldMatrix()
	{
		if (m_WorldMatrixDirty) 
		{
			auto position = GetPosition();
			auto scale = GetScale();
			auto rotation = GetRotation();
			auto translation = BuildTranslationMatrix(position);
			auto scaling = BuildScaleMatrix(scale);

			auto rx = BuildRotationMatrix(Vector3f(1, 0, 0), rotation.x());
			auto ry = BuildRotationMatrix(Vector3f(0, 1, 0), rotation.y());
			auto rz = BuildRotationMatrix(Vector3f(0, 0, 1), rotation.z());
			m_WorldMatrix = translation * rz * ry * rx * scaling; // make sure translation matrix go first.
			m_WorldMatrixDirty = false;
		}
		return m_WorldMatrix;
	}
}