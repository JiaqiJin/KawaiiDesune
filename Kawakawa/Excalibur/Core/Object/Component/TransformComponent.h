#pragma once

#include "../../../Interface/IComponent.h"
#include "../../../Core/Math/KawaiiMath.h"

namespace Excalibur
{
	class TransformComponent : public IComponent
	{
	public:
		TransformComponent();

		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;

		// Getters
		Vector3f	GetPosition() noexcept { return m_Position; }
		Quaternion	GetRotation() noexcept { return m_Rotation; }
		Vector3f	GetScale() noexcept { return m_Scale; }
		void		SetPosition(Vector3f position) { m_Position = position; m_WorldMatrixDirty = true; m_WorldMatrixInverseDirty = true; }
		void		SetRotation(Quaternion rotation);
		void		SetScale(Vector3f scale) { m_Scale = scale; m_WorldMatrixDirty = true; m_WorldMatrixInverseDirty = true; }
		Matrix4x4f	GetWorldMatrix();
		Matrix4x4f  GetWorldMatrixInverse();

	private:
		bool m_WorldMatrixDirty;
		bool m_WorldMatrixInverseDirty;
		Matrix4x4f	m_WorldMatrix;
		Matrix4x4f  m_WorldMatrixInverse;
		Vector3f m_Position;
		Quaternion m_Rotation;
		Vector3f m_Scale;
	};
}