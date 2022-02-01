#pragma once

#include "../Interface/IResourceh.h"

namespace Excalibur
{
	class IVertexBuffer : public IResouceBase
	{
	public:
		virtual void Initialize(void* data, unsigned int count, VertexFormat vf)
		{
			m_Count = count;
			m_VertexFormat = vf;
		}

		virtual void Finialize() = 0;

		virtual unsigned int GetVertexSize(VertexFormat vf)
		{
			if (vf == VertexFormat::VF_P3F || vf == VertexFormat::VF_N3F) {
				return sizeof(float) * 3;
			}
			else if (vf == VertexFormat::VF_C4B) {
				return 4;
			}
			else if (vf == VertexFormat::VF_T2F) {
				return sizeof(float) * 2;
			}
			else {
				return 0;
			}
		}

		unsigned int GetCount() { return m_Count; }
		VertexFormat GetVertexFormat() { return m_VertexFormat; }
	protected:
		unsigned int m_Count;
		VertexFormat m_VertexFormat;
	};

}