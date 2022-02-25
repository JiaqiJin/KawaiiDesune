#include "CameraComponent.h"
#include <DirectXMath.h>

using namespace DirectX;

namespace Excalibur
{
	CameraComponent::CameraComponent() :
		m_CameraType(CameraType::Perspective),
		m_Position(Vector3f(-10, 10, -20)),
		m_Lookat(Vector3f(0, 1, 0)),
		m_Up(Vector3f(0, 1, 0)),
		m_NearClip(0.01f),
		m_FarClip(1000.0f),
		m_Fov(PI / 3),
		m_ViewDirty(true),
		m_ProjectionDirty(true)
	{
	}

	int CameraComponent::Initialize()
	{
		return 0;
	}

	void CameraComponent::Finalize()
	{
	}

	const Matrix4f& Excalibur::CameraComponent::GetViewMatrix()
	{
		if (m_ViewDirty) {
			m_ViewMatrix = BuildViewLookatLH(m_Position, m_Lookat, m_Up);
			m_ViewDirty = false;
		}

		return m_ViewMatrix;
	}

	const Matrix4f& CameraComponent::GetPerspectiveMatrix()
	{
		float width = 1024.0f;
		float height = 768.0f;

		if (m_ProjectionDirty) {
			if (m_CameraType == CameraType::Orth) {
				m_ProjectionMatrix = BuildOrthoLH(width, height, m_NearClip, m_FarClip);
			}
			else
			{
				m_ProjectionMatrix = BuildPerspectiveFovLH(m_Fov, width / height, m_NearClip, m_FarClip);
			}
		}

		return m_ProjectionMatrix;
	}

}