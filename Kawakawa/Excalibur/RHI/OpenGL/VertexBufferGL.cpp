#include "VertexBufferGL.h"

#include <cassert>

namespace Excalibur
{
	VertexBufferGL::VertexBufferGL(void* data, uint32_t count, VertexFormat vf, int index)
		: m_Index(index)
	{
		Initialize(data, count, vf);
	}

	VertexBufferGL::~VertexBufferGL()
	{
		glDeleteBuffers(1, &mVBO);
	}

	void VertexBufferGL::Initialize(void* data, unsigned int count, VertexFormat vf)
	{
		m_VertexCount = count;

		size_t dataSize = GetVertexSize(vf) * count;
		glGenBuffers(1, &mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
		glEnableVertexAttribArray(m_Index);

		switch (vf)
		{
		case VF_None:
			assert(false);
		case VF_3F:
			glVertexAttribPointer(m_Index, 3, GL_FLOAT, false, 0, 0);
			break;
		case VF_2F:
			glVertexAttribPointer(m_Index, 2, GL_FLOAT, false, 0, 0);
			break;
		case VF_4B:
			glVertexAttribPointer(m_Index, 4, GL_FLOAT, false, 0, 0);
			break;
		default:
			assert(false);
		}
	}
}