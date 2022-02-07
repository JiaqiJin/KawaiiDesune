#include "TransformComponent.h"

namespace Excalibur
{
	TransformComponent::TransformComponent()
		: m_Position(0.0f, 0.0f, 0.0f),
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
			Vector3f position = GetPosition();
			Vector3f rotation = GetRotation();
			Vector3f scale = GetScale();
			auto translation = Math::BuildTranslationMatrix(position);
			auto scaling = Math::BuildScaleMatrix(scale);

			auto rx = Math::BuildRotationMatrix(Vector3f(1, 0, 0), rotation.x());
			auto ry = Math::BuildRotationMatrix(Vector3f(0, 1, 0), rotation.y());
			auto rz = Math::BuildRotationMatrix(Vector3f(0, 0, 1), rotation.z());
			m_WorldMatrix = translation * rz * ry * rx * scaling;
			m_WorldMatrixDirty = false;
		}

		return m_WorldMatrix;
	}
}