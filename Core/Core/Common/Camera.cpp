#include "Camera.h"

Camera::Camera(CameraDesc const& desc)
	: m_position{ desc.pos_x, desc.pos_y, desc.pos_z}
	, m_rightVector{ 1.0f,0.0f,0.0f }, m_worldUp{ 0.0f,1.0f,0.0f }, m_lookVector{ 0.0f,0.0f,1.0f },
	m_aspectRatio{desc.aspectRatio}, m_fov{desc.fov}, m_near{desc.near}, m_far{desc.far}
{
	SetView();
}

DirectX::XMVECTOR Camera::Position() const
{
	return DirectX::XMLoadFloat3(&m_position);
}

DirectX::XMMATRIX Camera::View() const
{
	return DirectX::XMLoadFloat4x4(&m_viewMatrix);
}

DirectX::XMMATRIX Camera::Proj() const
{
	return DirectX::XMLoadFloat4x4(&m_projMatrix);
}

DirectX::XMMATRIX Camera::ViewProj() const
{
	return DirectX::XMMatrixMultiply(View(), Proj());
}

float Camera::Near() const
{
	return m_near;
}

float Camera::Far() const
{
	return m_far;
}

float Camera::Fov() const
{
	return m_fov;
}

float Camera::AspectRatio() const
{
	return m_aspectRatio;
}

void Camera::UpdateViewMatrix()
{
	DirectX::XMVECTOR R = DirectX::XMLoadFloat3(&m_rightVector);
	DirectX::XMVECTOR U = DirectX::XMLoadFloat3(&m_worldUp);
	DirectX::XMVECTOR L = DirectX::XMLoadFloat3(&m_lookVector);
	DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&m_position);

	// Keep camera's axes orthogonal to each other and of unit length.
	L = DirectX::XMVector3Normalize(L);
	U = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = DirectX::XMVector3Cross(U, L);

	// Fill in the view matrix entries.
	DirectX::XMStoreFloat3(&m_rightVector, R);
	DirectX::XMStoreFloat3(&m_worldUp, U);
	DirectX::XMStoreFloat3(&m_lookVector, L);

	SetView();
}

void Camera::SetView()
{
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&m_position);
	DirectX::XMVECTOR look = DirectX::XMLoadFloat3(&m_lookVector);
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&m_worldUp);
	DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(pos, look, up);
	DirectX::XMStoreFloat4x4(&m_viewMatrix, view);
}