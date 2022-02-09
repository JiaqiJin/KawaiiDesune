
#pragma once
#include <set>
#include "../../../Interface/ISystem.h"
#include "../Component/MeshRenderComponent.h"
#include "../../../RHI/GraphicsMgr.h"
#include "../../../RHI/MeshBase.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

using namespace std;

namespace Excalibur
{
	class World;

	class MeshRenderSystem : public ITickableSystem 
	{
	public:
		MeshRenderSystem(World* world);

		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;
		virtual void Tick() noexcept;

		void Render();
		void LoadMesh(aiMesh* mesh, const aiScene* scene);

		void AddComponent(MeshRenderComponent* comp);
		void DeleteComponent(MeshRenderComponent* comp);
	public:
		std::vector<std::shared_ptr<IMesh>> m_Meshes;
	private:
		World* m_World;
		GraphicsManager* m_GraphicsManager;
		std::set<MeshRenderComponent*> m_Components;
	};
}