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

	class RenderMesh : public IRenderResource 
	{
	public:
		RenderMesh();
		virtual ~RenderMesh();
		virtual void Initialize(void* data, int count, VertexFormat vf);
		virtual void Render(World* world, const Matrix4f& worldMatrix);
		virtual int	GetVaildVertexBufferCount();
		virtual void Finialize();
		virtual shared_ptr<Material> GetMaterial();

	public:
		shared_ptr<VertexBuffer>	m_Positions;
		shared_ptr<VertexBuffer>	m_Normals;
		shared_ptr<VertexBuffer>	m_TexCoords;
		shared_ptr<IndexBuffer>		m_Indexes;
		PrimitiveType				m_Type;
		shared_ptr<Material>		m_Material;
	};

}