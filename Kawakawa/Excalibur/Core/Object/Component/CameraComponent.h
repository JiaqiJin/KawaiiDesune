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

		const Matrix4f& GetViewMatrix();
		const Matrix4f& GetPerspectiveMatrix();

		CameraType GetType() { return m_CameraType; }
		void SetType(CameraType type) { m_CameraType = type; m_ViewDirty = true; m_ProjectionDirty = true; }

		Vector3f GetPosition() { return m_Position; }
		Vector3f GetLookat() { return m_Lookat; }
		Vector3f GetUp() { return m_Up; }
		void SetPosition(const Vector3f& position) { m_Position = position; m_ViewDirty = true; }
		void SetLook(const Vector3f& lookat) { m_Lookat = lookat; m_ViewDirty = true; }
		void SetUp(const Vector3f& up) { m_Up = up; m_ViewDirty = true; }

		float GetNearClip() { return m_NearClip; }
		float GetFarClip() { return m_FarClip; }
		float GetFov() { return m_Fov; }
		void SetNearClip(float n) { m_ProjectionDirty = true; m_NearClip = n; }
		void SetFarClip(float f) { m_ProjectionDirty = true; m_FarClip = f; }
		void SetFov(float fov) { m_Fov = fov; m_ProjectionDirty = true; }

	private:
		CameraType m_CameraType;

		Matrix4f m_ViewMatrix;
		Matrix4f m_ProjectionMatrix;
		Vector3f m_Position;
		Vector3f m_Lookat;
		Vector3f m_Up;

		float m_NearClip;
		float m_FarClip;
		float m_Fov;

		bool m_ViewDirty;
		bool m_ProjectionDirty;
	};
}