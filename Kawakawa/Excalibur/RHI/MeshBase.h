#pragma once

#include "../Interface/IResourceh.h"
#include "VertexBufferBase.h"
#include "IndexBufferBase.h"
#include "MaterialBase.h"
#include "TexturehBase.h"
#include "../Core/Math/EigenMath.h"
#include "../Core/Math/KawaiiMath.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>     
#include <assimp/postprocess.h>

#include <memory>

namespace Excalibur
{
	class World;
	class Entity;

	enum TerrainLod {
		Low,
		Middle,
		High
	};

	enum MeshType {
		MT_Model = 1,
		MT_Skybox = 2,
		MT_UI = 3,
		MT_TERRAIN = 4,
	};

	enum PrimitiveType {
		PT_POINT = 1,
		PT_LINE,
		PT_TRIANGLE,
	};

	class IMesh : public IResouceBase
	{
	public:
		virtual void Initialize(aiMesh* mesh) noexcept = 0;
		virtual void Initialize(void* data, int count, VertexFormat vf) noexcept = 0;
		virtual void InitializeUI() noexcept = 0;
		virtual void InitializeTerrain() noexcept = 0;

		virtual void Render(Entity* self) noexcept = 0;
		virtual void Render(const Matrix4x4f& world, const Matrix4x4f& view, const Matrix4x4f& projection) noexcept = 0;

		IMesh() :
			mMeshType(MeshType::MT_Model),
			mPrimitiveType(PrimitiveType::PT_TRIANGLE),
			mPositions(nullptr),
			mNormals(nullptr),
			mTexCoords(nullptr),
			mIndexes(nullptr),
			mBoneIdxes(nullptr),
			mBoneWeights(nullptr),
			mMaterial(nullptr) {}

		std::shared_ptr<IMaterial>	GetMaterial() { return mMaterial; }

	public:
		MeshType					mMeshType;
		PrimitiveType				mPrimitiveType;

		std::shared_ptr<IVertexBuffer>	mPositions;
		std::shared_ptr<IVertexBuffer>	mNormals;
		std::shared_ptr<IVertexBuffer>	mTexCoords;
		std::shared_ptr<IVertexBuffer>  mBoneIdxes;
		std::shared_ptr<IVertexBuffer>  mBoneWeights;
		std::shared_ptr<IIndexBuffer>	mIndexes;
		std::shared_ptr<IMaterial>		mMaterial;
	};
}