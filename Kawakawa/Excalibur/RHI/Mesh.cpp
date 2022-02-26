//#include "RenderMesh.h"
//#include "../Core/Object/World.h"
//#include "GraphicsMgr.h"
//
//namespace Excalibur
//{
//	Excalibur::RenderMesh::RenderMesh()
//		: m_Type(PrimitiveType::PT_POINT),
//		m_Positions(nullptr),
//		m_Normals(nullptr),
//		m_TexCoords(nullptr),
//		m_Indexes(nullptr)
//	{
//
//	}
//
//	RenderMesh::~RenderMesh()
//	{
//
//	}
//
//	void RenderMesh::Initialize(void* data, int count, VertexFormat vf)
//	{
//
//	}
//
//	void RenderMesh::Render(World* world, const Matrix4f& worldMatrix) 
//	{
//
//	}
//
//	int RenderMesh::GetVaildVertexBufferCount() 
//	{
//		int result = 0;
//		if (m_Positions) {
//			result += 1;
//		}
//		if (m_Normals) {
//			result += 1;
//		}
//		if (m_TexCoords) {
//			result += 1;
//		}
//		return result;
//	}
//
//	void RenderMesh::Finialize()
//	{
//
//	}
//
//	shared_ptr<Material> RenderMesh::GetMaterial()
//	{
//		return m_Material;
//	}
//
//}