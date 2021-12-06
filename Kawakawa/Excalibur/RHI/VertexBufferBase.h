#pragma once

#include "../Interface/IResourceh.h"

namespace Excalibur
{
	enum VertexFormat
	{
		VF_None = 0,
		VF_2F,
		VF_3F,
		VF_4B,
		VF_4I,
		VF_4F,
	};

	class VertexBufferBase : public IResouceBase
	{
	public:
		VertexBufferBase() : m_VertexCount(0), m_VertexFormat(VertexFormat::VF_None) {}

		virtual void Initialize(void* data, uint32_t count, VertexFormat vf) = 0;

		unsigned int GetVertexSize(VertexFormat vf)
		{
			if (vf == VertexFormat::VF_3F) {
				return sizeof(float) * 3;
			}
			else if (vf == VertexFormat::VF_4B) {
				return 4;
			}
			else if (vf == VertexFormat::VF_2F) {
				return sizeof(float) * 2;
			}
			else if (vf == VertexFormat::VF_4I) {
				return sizeof(int) * 4;
			}
			else if (vf == VertexFormat::VF_4F) {
				return sizeof(float) * 4;
			}
			else {
				return 0;
			}
		}

		unsigned int GetVertextCount() { return m_VertexCount; }
	protected:
		uint32_t m_VertexCount;
		VertexFormat m_VertexFormat;
	};
}