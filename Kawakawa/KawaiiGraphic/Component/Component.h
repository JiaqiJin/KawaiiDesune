#pragma once

#include "Math/Transform.h"

class Component
{
public:
	Component() {}
	virtual ~Component() {}

	TVector3 GetWorldLocation() const{ return m_WorldTransform.Location; }
	TTransform GetWorldTransform() const { return m_WorldTransform; }
	TRotator GetWorldRotation() const{ return m_WorldTransform.Rotation; }

	void SetWorldTransform(const TTransform& Transform){ m_WorldTransform = Transform; }
	virtual void SetWorldLocation(const TVector3& Location) { m_WorldTransform.Location = Location; }
	virtual void SetWorldRotation(const TRotator& Rotation) { m_WorldTransform.Rotation = Rotation; }


protected:
	TTransform m_WorldTransform;
};