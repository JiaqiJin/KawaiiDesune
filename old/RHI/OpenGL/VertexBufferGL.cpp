#include "VertexBufferGL.h"

#include <cassert>

namespace Excalibur
{
	VertexBufferGL::VertexBufferGL(void* data, unsigned int count, VertexFormat vf, int index) : m_Index(index)
	{
		Initialize(data, count, vf);
	}

	VertexBufferGL::~VertexBufferGL()
	{
		Finialize();
	}

	void VertexBufferGL::Initialize(void* data, unsigned int count, VertexFormat vf)
	{
		IVertexBuffer::Initialize(data, count, vf);

		size_t dataSize = GetVertexSize(vf) * count;
		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
		glEnableVertexAttribArray(m_Index);

		switch (vf)
		{
		case VF_None:
			assert(false);
		case VF_P3F:
			glVertexAttribPointer(m_Index, 3, GL_FLOAT, false, 0, 0);
			break;
		case VF_T2F:
			glVertexAttribPointer(m_Index, 2, GL_FLOAT, false, 0, 0);
			break;
		case VF_N3F:
			glVertexAttribPointer(m_Index, 3, GL_FLOAT, false, 0, 0);
			break;
		case VF_C4B:
			glVertexAttribPointer(m_Index, 4, GL_FLOAT, false, 0, 0);
			break;
		default:
			assert(false);
		}
	}

	void VertexBufferGL::Finialize()
	{
		glDeleteBuffers(1, &m_VBO);
	}

}