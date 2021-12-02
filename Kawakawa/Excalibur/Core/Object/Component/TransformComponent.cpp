#include "TransformComponent.h"

namespace Excalibur
{
	TransformComponent::TransformComponent() :
		m_Position(0.0f, 0.0f, 0.0f),
		m_Rotation(0.0f, 0.0f, 0.0f, 1.0f),
		m_Scale(1.0f, 1.0f, 1.0f),
		m_WorldMatrixDirty(true),
		m_WorldMatrixInverseDirty(true)
	{
	
	}

	int TransformComponent::Initialize() noexcept
	{
		return 0;
	}


	void TransformComponent::Finalize() noexcept
	{

	}

	void TransformComponent::SetRotation(Quaternion rotation)
	{
		Normalize(rotation);
		m_Rotation = rotation;
		m_WorldMatrixDirty = true;
		m_WorldMatrixInverseDirty = true;
	}

	Matrix4x4f TransformComponent::GetWorldMatrix()
	{
		return m_WorldMatrix;
	}

	Matrix4x4f TransformComponent::GetWorldMatrixInverse()
	{
		return m_WorldMatrix;
	}
}