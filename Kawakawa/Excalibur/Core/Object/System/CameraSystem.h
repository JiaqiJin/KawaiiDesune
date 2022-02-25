#pragma once
#include <memory>
#include "../Entity.h"
#include "../../../Interface/ISystem.h"

namespace Excalibur {
	class World;

	class CameraSystem : public IModule 
	{
	public:
		CameraSystem(World* world);
		virtual	int	Initialize();
		virtual	void Finalize();
	public:
		std::shared_ptr<Entity>	GetMainCamera();
		void SetMainCamera(std::shared_ptr<Entity> camera);

	private:
		World* m_World;
		std::shared_ptr<Entity>	m_MainCamera;
	};

}