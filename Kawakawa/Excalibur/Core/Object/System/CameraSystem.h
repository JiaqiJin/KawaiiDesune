#pragma once

#include "../../../Interface/ISystem.h"
#include "../Entity.h"

namespace Excalibur
{
	class World;

	class CameraSystem : public IModule
	{
	public:
		CameraSystem(World* world);
		virtual	int	Initialize() noexcept;
		virtual	void Finalize() noexcept;

		std::shared_ptr<Entity>	GetMainCamera();
		void SetMainCamera(std::shared_ptr<Entity> camera);
	private:
		World* m_World;
		std::shared_ptr<Entity> m_MainCamera;
	};
}