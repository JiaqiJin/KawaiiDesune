#pragma once

#include "../../../Interface/IComponent.h"
#include "../../Math/KawaiiMath.h"

namespace Excalibur
{
	class TransformComponent : public IComponent
	{
	public:
		virtual int Initialize();
		virtual void Finalize();

	public:
		TransformComponent();
		Vector3f GetPosition() { return m_Position; }
		Vector3f GetRotation() { return m_Rotation; }
		Vector3f GetScale() { return mScale; }
		void SetPosition(Vector3f position) { m_Position = position; m_WorldMatrixDirty = true; }
		void SetRotation(Vector3f rotation) { m_Rotation = rotation; m_WorldMatrixDirty = true; }
		void SetScale(Vector3f scale) { mScale = scale; m_WorldMatrixDirty = true;}
		Matrix4f GetWorldMatrix();

	private:
		bool m_WorldMatrixDirty;
		Matrix4f m_WorldMatrix;
		Vector3f m_Position;
		Vector3f m_Rotation;
		Vector3f mScale;
	};

}