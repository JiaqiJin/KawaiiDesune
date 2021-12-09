#include "CameraComponent.h"

namespace Excalibur
{
	CameraComponent::CameraComponent()
		: m_CameraType(CameraType::Perspective),
		m_Position(Vector3f(30, 30, 40)),
		m_LookAt(Vector3f(0, 0, 0)),
		m_Up(Vector3f(0, 1, 0)),
		m_NearClip(0.01f),
		m_FarClip(2000.0f),
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

	const Matrix4x4f CameraComponent::GetViewMatrix()
	{
		if (m_ViewDirty) {
			BuildMatrixViewLookatLH(m_ViewMatrix, m_Position, m_LookAt, m_Up);
			m_ViewDirty = false;
		}

		return m_ViewMatrix;
	}

	const Matrix4x4f CameraComponent::GetViewMatrixOrigin()
	{
		auto m = GetViewMatrix();
		m[0][3] = 0.0f;
		m[1][3] = 0.0f;
		m[2][3] = 0.0f;
		return m;
	}

	const Matrix4x4f CameraComponent::GetPerspectiveMatrix()
	{
		float width = 1024.0f;
		float height = 768.0f;

		BuildMatrixPerspectiveFovLH(m_ProjectionMatrix, m_Fov, width / height, m_NearClip, m_FarClip);

		return m_ProjectionMatrix;
	}
}