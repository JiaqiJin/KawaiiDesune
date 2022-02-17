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

		virtual std::shared_ptr<IMaterial> GetMaterial() { return m_Material; }
	public:
		std::shared_ptr<IVertexBuffer>	m_Positions;
		std::shared_ptr<IVertexBuffer>	m_Normals;
		std::shared_ptr<IVertexBuffer>	m_TexCoords;
		std::shared_ptr<IIndexBuffer>	m_Indexes;
		PrimitiveType				m_Type;
		std::shared_ptr<IMaterial>		m_Material;
	};
}