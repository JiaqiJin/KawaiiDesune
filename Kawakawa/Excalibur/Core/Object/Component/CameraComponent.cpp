#include "CameraComponent.h"

namespace Excalibur
{
	CameraComponent::CameraComponent() :
		mCameraType(CameraType::Perspective),
		mPosition(Vector3f(30, 30, 40)),
		mLookat(Vector3f(0, 0, 0)),
		mUp(Vector3f(0, 1, 0)),
		mNearClip(0.01f),
		mFarClip(2000.0f),
		mFov(PI / 3),
		mViewDirty(true),
		mProjectionDirty(true),
		mSpeed(1.0f)
	{
		
	}

	int CameraComponent::Initialize() noexcept
	{
		return 0;
	}

	void CameraComponent::Finalize() noexcept
	{
	}

	const Matrix4x4f CameraComponent::GetViewMatrix()
	{
		if (mViewDirty) {
			BuildMatrixViewLookatLH(mViewMatrix, mPosition, mLookat, mUp);
			mViewDirty = false;
		}

		return mViewMatrix;
	}

	const Matrix4x4f CameraComponent::GetViewMatrixOrigin()
	{
		auto m = GetViewMatrix();
		m[0][3] = 0.0f;
		m[1][3] = 0.0f;
		m[2][3] = 0.0f;
		return m;
	}

	const Matrix4x4f CameraComponent::GetPerspectiveMatrix()
	{
		if (mProjectionDirty) {
			float width = 1024.0f;
			float height = 768.0f;
			if (mCameraType == CameraType::Orth) {
				BuildMatrixOrthoLH(mProjectionMatrix, width, height, mNearClip, mFarClip);
			}
			else
			{
				BuildMatrixPerspectiveFovLH(mProjectionMatrix, mFov, width / height, mNearClip, mFarClip);
			}
			mProjectionDirty = false;
		}
		return mProjectionMatrix;
	}

	void CameraComponent::Forward()
	{
	
	}

	void CameraComponent::Backward()
	{
	
	}

	void CameraComponent::MoveLeft()
	{
		
	}

	void CameraComponent::MoveRight()
	{
		
	}

}