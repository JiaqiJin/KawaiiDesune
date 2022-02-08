#include "CameraSystem.h"

namespace Excalibur
{
	CameraSystem::CameraSystem(World* world)
		: m_World(world)
	{

	}

	int	CameraSystem::Initialize() noexcept
	{
		return 0;
	}

	void CameraSystem::Finalize() noexcept
	{
		m_MainCamera = nullptr;
	}

	std::shared_ptr<Entity>	CameraSystem::GetMainCamera()
	{
		return m_MainCamera;
	}

	void CameraSystem::SetMainCamera(std::shared_ptr<Entity> camera)
	{
		m_MainCamera = camera;
	}
}