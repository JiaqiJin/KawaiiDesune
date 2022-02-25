#include "CameraSystem.h"

namespace Excalibur
{
	CameraSystem::CameraSystem(World* world) :
		m_World(world)
	{
	}

	int CameraSystem::Initialize()
	{
		return 0;
	}

	void CameraSystem::Finalize()
	{
		m_MainCamera = nullptr;
	}

	std::shared_ptr<Entity> CameraSystem::GetMainCamera()
	{
		return m_MainCamera;
	}

	void CameraSystem::SetMainCamera(std::shared_ptr<Entity> camera)
	{
		m_MainCamera = camera;
	}

}