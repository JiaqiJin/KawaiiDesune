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

	class IMesh : public IResouceBase
	{
	public:
		IMesh() {}
		virtual ~IMesh() {}
		virtual void Initialize(void* data, int count, VertexFormat vf) = 0;
		virtual void Render(World* world, const Matrix4f& worldMatrix) = 0;
		virtual void Finialize() = 0;

		virtual std::shared_ptr<IMaterial> GetMaterial() { return mMaterial; }
	public:
		std::shared_ptr<IVertexBuffer>	mPositions;
		std::shared_ptr<IVertexBuffer>	mNormals;
		std::shared_ptr<IVertexBuffer>	mTexCoords;
		std::shared_ptr<IIndexBuffer>	mIndexes;
		PrimitiveType				mType;
		std::shared_ptr<IMaterial>		mMaterial;
	};
}