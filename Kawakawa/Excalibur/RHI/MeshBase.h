#pragma once

#include "../Interface/IResourceh.h"
#include "VertexBufferBase.h"
#include "IndexBufferBase.h"
#include "../Core/Math/EigenMath.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>     
#include <assimp/postprocess.h>

#include <memory>

namespace Excalibur
{
	class World;
	class Entity;

	class MeshBase : public IResouceBase
	{
	public:
		MeshBase() : m_Position(nullptr), m_Indexes(nullptr), m_Normal(nullptr) {}
		virtual void Initialize(aiMesh* mesh) = 0;
		virtual void Initialize(void* data, int count, VertexFormat vf) = 0;
		virtual void Render(World* world, const Matrix4f& worldMatrix) = 0;
		
	protected:
		std::shared_ptr<VertexBufferBase> m_Position;
		std::shared_ptr<VertexBufferBase> m_Normal;
		std::shared_ptr<IndexBufferBase> m_Indexes;
	};
}