#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <DirectXCollision.h>

struct CameraDesc
{
	// Default camera values
	float aspectRatio;
	float near;
	float far;
	float fov;
	float pos_x, pos_y, pos_z;
};

class Camera
{
	const float SPEED = 2.5f;
	const float SENSITIVITY = 0.1f;
public:

	Camera() = default;
	Camera(CameraDesc const& desc);

	DirectX::XMVECTOR Position() const;
	DirectX::XMMATRIX View() const;
	DirectX::XMMATRIX Proj() const;
	DirectX::XMMATRIX ViewProj() const;

	DirectX::XMVECTOR Up() const
	{
		return DirectX::XMLoadFloat3(&m_worldUp);
	}

	DirectX::XMVECTOR Right() const
	{
		return DirectX::XMLoadFloat3(&m_rightVector);
	}

	DirectX::XMVECTOR Forward() const
	{
		return DirectX::XMLoadFloat3(&m_lookVector);
	}

	float Near() const;

	float Far() const;

	float Fov() const;

	float AspectRatio() const;

private:
	void UpdateViewMatrix();

	void SetView();

private:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rightVector;
	DirectX::XMFLOAT3 m_worldUp;
	DirectX::XMFLOAT3 m_lookVector;
	DirectX::XMFLOAT4X4 m_viewMatrix;
	DirectX::XMFLOAT4X4 m_projMatrix;

	float m_aspectRatio;
	float m_fov;
	float m_near, m_far;
};

#endif