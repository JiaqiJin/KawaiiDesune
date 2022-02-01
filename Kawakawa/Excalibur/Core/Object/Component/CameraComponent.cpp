#include "CameraComponent.h"

namespace Excalibur
{
	CameraComponent::CameraComponent()
	{

	}

	int CameraComponent::Initialize() noexcept
	{
		return 0;
	}

	void CameraComponent::Finalize() noexcept
	{

	}

	const Matrix4f& CameraComponent::GetViewMatrix()
	{
		return m_ViewMatrix;
	}

	const Matrix4f& CameraComponent::GetPerspectiveMatrix()
	{
		return m_ProjectionMatrix;
	}

}