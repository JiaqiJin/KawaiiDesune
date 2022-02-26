#pragma once

#include "../Interface/IResource.h"

namespace Excalibur 
{
	class IVertexBuffer : public IRenderResource 
	{
	public:
		IVertexBuffer() : m_VertexCount(0), m_VertexFormat(VertexFormat::VF_None) {}

		virtual void Initialize(void* data, unsigned int count, VertexFormat vf) = 0;
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

		unsigned int GetVertexCount() { return m_VertexCount; }

	protected:
		unsigned int m_VertexCount;
		VertexFormat m_VertexFormat;
	};

}