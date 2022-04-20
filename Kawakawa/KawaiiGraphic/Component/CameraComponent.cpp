#include "CameraComponent.h"

CameraComponent::CameraComponent()
{
	SetFrustum(0.25f * TMath::Pi, 1.0f, 0.1f, 100.0f);
}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::SetWorldLocation(const TVector3& Location)
{
	m_WorldTransform.Location = Location;
	ViewDirty = true;
}

float CameraComponent::GetNearZ()const
{
	return NearZ;
}

float CameraComponent::GetFarZ()const
{
	return FarZ;
}

float CameraComponent::GetAspect()const
{
	return Aspect;
}

float CameraComponent::GetFovY()const
{
	return FovY;
}

float CameraComponent::GetFovX()const
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return float(2.0f * atan(halfWidth / NearZ));
}
	
void CameraComponent::SetWorldLocation(const TVector3& Location)
{
	m_WorldTransform.Location = Location;
	ViewDirty = true;
}

float CameraComponent::GetNearZ()const
{
	return NearZ;
}

float CameraComponent::GetFarZ()const
{
	return FarZ;
}

float CameraComponent::GetAspect()const
{
	return Aspect;
}

float CameraComponent::GetFovY()const
{
	return FovY;
}

float CameraComponent::GetFovX()const
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return float(2.0f * atan(halfWidth / NearZ));
}

float CameraComponent::GetNearWindowWidth() const
{
	return Aspect * NearWindowHeight;
}

float CameraComponent::GetNearWindowHeight() const
{
	return NearWindowHeight;
}

float CameraComponent::GetFarWindowWidth()const
{
	return Aspect * FarWindowHeight;
}

float CameraComponent::GetFarWindowHeight()const
{
	return FarWindowHeight;
}

void CameraComponent::LookAt(const TVector3& pos, const TVector3& target, const TVector3& up)
{

}

TMatrix CameraComponent::GetView()const
{

}

TMatrix CameraComponent::GetProj()const
{

}

void CameraComponent::MoveRight(float Dist)
{

}

void CameraComponent::MoveForward(float Dist)
{

}

void CameraComponent::MoveUp(float Dist)
{

}

void CameraComponent::Pitch(float Degrees)
{

}

void CameraComponent::RotateY(float Degrees)
{

}

void CameraComponent::UpdateViewMatrix()
{

}

void CameraComponent::SetPrevViewProj(const TMatrix& VP)
{

}

TMatrix CameraComponent::GetPrevViewProj() const
{

}
void CameraComponent::SetFrustum(float fovY, float aspect, float zn, float zf)
{
	FovY = fovY;
	Aspect = aspect;
	NearZ = zn;
	FarZ = zf;

	NearWindowHeight = 2.0f * NearZ * tanf(0.5f * FovY);
	FarWindowHeight = 2.0f * FarZ * tanf(0.5f * FovY);

	Proj = TMatrix::CreatePerspectiveFieldOfView(FovY, Aspect, NearZ, FarZ);
}