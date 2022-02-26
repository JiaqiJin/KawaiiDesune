#pragma once
#include <set>

#include "../../../Interface/ISystem.h"
#include "../../../RHI/GraphicsMgr.h"
#include "../../../RHI/Mesh.h"

#include "../Components/MeshRenderComponent.h"

using namespace std;

namespace Excalibur {
	class World;

	class MeshRenderSystem : public ITickableSystem
	{

	public:
		MeshRenderSystem(World* world);

		virtual int Initialize();
		virtual void Finalize();
		virtual void Tick();

	public:
		virtual void AddComponent(MeshRenderComponent* comp);
		virtual void DeleteComponent(MeshRenderComponent* comp);
		virtual void Render();

		virtual void LoadMesh(aiMesh* mesh, const aiScene* world);

	public:
		std::vector<shared_ptr<IMesh>>	m_Meshes;
	private:
		World* m_World;
		IGraphicsManager* m_GraphicsManager;
		std::set<MeshRenderComponent*> m_Components;

		

	};
}