
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

	class MeshRenderSystem : public ITickableSystem {

	public:
		MeshRenderSystem(World* world);

		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;
		virtual void Tick() noexcept;

	public:
		virtual void AddComponent(MeshRenderComponent* comp);
		virtual void DeleteComponent(MeshRenderComponent* comp);
		virtual void Render();
		
		void LoadMesh(aiMesh* mesh, const aiScene* world);

	public:
		std::vector<shared_ptr<MeshBase>> mMeshes;

	private:
		GraphicsManager* mGraphicsManager;
		World* mWorld;
		std::set<MeshRenderComponent*> mComponents;
	};
}