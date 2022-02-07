#pragma once

#include "../../../Interface/IComponent.h"
#include "../../../Core/Math/EigenMath.h"

namespace Excalibur
{
	class TransformComponent : public IComponent
	{
	public:
		TransformComponent();

		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;

		Vector3f GetPosition() noexcept { return m_Position; }
		Vector3f GetRotation() noexcept { return m_Rotation; }
		Vector3f GetScale() noexcept { return m_Scale; }
		Matrix4f GetWorldMatrix();
		void SetPosition(Vector3f position) { m_Position = position; m_WorldMatrixDirty = true; }
		void SetRotation(Vector3f rotation) { m_Rotation = rotation; m_WorldMatrixDirty = true; }
		void SetScale(Vector3f scale) { m_Scale = scale; m_WorldMatrixDirty = true; }
	private:
		bool m_WorldMatrixDirty;
		Matrix4f m_WorldMatrix;
		Vector3f m_Position;
		Vector3f m_Rotation;
		Vector3f m_Scale;
	};
}