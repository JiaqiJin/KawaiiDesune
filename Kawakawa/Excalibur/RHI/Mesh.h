#pragma once
#include <memory>

#include "../Interface/IResource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>       


using namespace std;

namespace Excalibur 
{
	class World;

	class IMesh : public IRenderResource 
	{
	public:
		IMesh() : m_Type(PrimitiveType::PT_POINT),
			m_Positions(nullptr),
			m_Normals(nullptr),
			m_TexCoords(nullptr),
			m_Indexes(nullptr) {}

		virtual ~IMesh() {}
		virtual void Initialize(aiMesh* mesh) = 0;
		virtual void Initialize(void* data, int count, VertexFormat vf) = 0;
		virtual void Render(World* world, const Matrix4f& worldMatrix) = 0;
		virtual void Finialize() = 0;

		virtual shared_ptr<IMaterial> GetMaterial() { return m_Material; }

		virtual int GetVaildVertexBufferCount()
		{
			int result = 0;
			if (m_Positions) {
				result += 1;
			}
			if (m_Normals) {
				result += 1;
			}
			if (m_TexCoords) {
				result += 1;
			}
			return result;
		}
	public:
		shared_ptr<IVertexBuffer>	m_Positions;
		shared_ptr<IVertexBuffer>	m_Normals;
		shared_ptr<IVertexBuffer>	m_TexCoords;
		shared_ptr<IIndexBuffer>	m_Indexes;
		PrimitiveType				m_Type;
		shared_ptr<IMaterial>		m_Material;
	};

}