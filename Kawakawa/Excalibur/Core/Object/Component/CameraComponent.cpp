#include "CameraComponent.h"

namespace Excalibur
{
	CameraComponent::CameraComponent()
		: m_CameraType(CameraType::Perspective),
		m_Position(Vector3f(-15, 20, -20)),
		m_LookAt(Vector3f(0, 1, 0)),
		m_Up(Vector3f(0, 1, 0)),
		m_NearClip(0.01f),
		m_FarClip(100000.0f),
		m_Fov(PI / 3),
		m_ViewDirty(true)
	{

	}

	int CameraComponent::Initialize() noexcept
	{
		return 0;
	}

	void CameraComponent::Finalize() noexcept
	{

	}

	const Matrix4f CameraComponent::GetViewMatrix()
	{
		if (m_ViewDirty) {
			m_ViewMatrix = BuildViewLookatLH(m_Position, m_LookAt, m_Up);
			m_ViewDirty = false;
		}

		return m_ViewMatrix;
	}

	const Matrix4f CameraComponent::GetPerspectiveMatrix()
	{
		float width = 1024.0f;
		float height = 768.0f;

		m_ProjectionMatrix = BuildPerspectiveFovLH(m_Fov, width / height, m_NearClip, m_FarClip);

		return m_ProjectionMatrix;
	}
}