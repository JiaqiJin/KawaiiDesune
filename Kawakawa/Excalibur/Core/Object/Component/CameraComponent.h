#pragma once

#include "../../../Interface/IComponent.h"
#include "../../../Core/Math/EigenMath.h"

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

		const Matrix4f GetViewMatrix();
		const Matrix4f GetPerspectiveMatrix();

	private:
		CameraType m_CameraType;

		bool m_ViewDirty;
		Matrix4f m_ViewMatrix;
		Vector3f m_Position;
		Vector3f m_LookUp;
		Vector3f m_LookAt;
		Vector3f m_Up;
	
		float m_NearClip;
		float m_FarClip;
		float m_Fov;

		Matrix4f m_ProjectionMatrix;
	};
}