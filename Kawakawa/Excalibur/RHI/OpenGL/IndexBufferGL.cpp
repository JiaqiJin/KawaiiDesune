#include "IndexBufferGL.h"

namespace Excalibur
{
	IndexBufferGL::IndexBufferGL(void* data, uint32_t count)
	{
		m_IndexCount = count;
		Initialize(data, count);
	}

	IndexBufferGL::~IndexBufferGL()
	{
		glDeleteBuffers(1, &mEBO);
	}

	void IndexBufferGL::Initialize(void* data, unsigned int count)
	{
		glGenBuffers(1, &mEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * 4, data, GL_STATIC_DRAW);	// count * 4 == datasize
	}
}