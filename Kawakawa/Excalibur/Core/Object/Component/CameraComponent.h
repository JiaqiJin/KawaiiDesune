#pragma once

#include "../../../Interface/IComponent.h"
#include "../../../Core/Math/KawaiiMath.h"

namespace Excalibur
{
	enum CameraType
	{
		Orth = 1,
		Perspective = 2
	};

	class CameraComponent : public IComponent
	{
	public:
		CameraComponent();
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;

		const Matrix4x4f GetViewMatrix();
		const Matrix4x4f GetPerspectiveMatrix();
		const Matrix4x4f GetViewMatrixOrigin();
	private:
		CameraType m_CameraType;

		bool m_ViewDirty;
		Matrix4x4f m_ViewMatrix;
		Vector3f m_Position;
		Vector3f m_LookUp;
		Vector3f m_LookAt;
		Vector3f m_Up;
	
		float m_NearClip;
		float m_FarClip;
		float m_Fov;

		Matrix4x4f	m_ProjectionMatrix;
	};
}