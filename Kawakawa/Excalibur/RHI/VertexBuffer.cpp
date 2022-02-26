//#include "VertexBuffer.h"
//#include "GraphicsMgr.h"
//
//namespace Excalibur
//{
//	void VertexBuffer::Initialize(void* data, unsigned int count, VertexFormat vf)
//	{
//		m_Count = count;
//		m_VertexFormat = vf;
//	}
//
//	unsigned int VertexBuffer::GetVertexSize(VertexFormat vf)
//	{
//		if (vf == VertexFormat::VF_P3F || vf == VertexFormat::VF_N3F)
//		{
//			return sizeof(float) * 3;
//		}
//		else if (vf == VertexFormat::VF_C4B) 
//		{
//			return 4;
//		}
//		else if (vf == VertexFormat::VF_T2F) 
//		{
//			return sizeof(float) * 2;
//		}
//		else 
//		{
//			return 0;
//		}
//	}
//
//	void VertexBuffer::Finialize()
//	{
//	}
//}