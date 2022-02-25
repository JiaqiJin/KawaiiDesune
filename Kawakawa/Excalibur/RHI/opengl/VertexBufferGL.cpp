#include "VertexBufferGL.h"


Excalibur::VertexBufferGL::VertexBufferGL(void * data, unsigned int count, VertexFormat vf, int index): mIndex(index)
{
	Initialize(data, count, vf);
}

Excalibur::VertexBufferGL::~VertexBufferGL()
{
}

void Excalibur::VertexBufferGL::Initialize(void * data, unsigned int count, VertexFormat vf)  
{
	VertexBuffer::Initialize(data, count, vf);

	size_t dataSize = GetVertexSize(vf) * count;
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(mIndex);

	switch (vf)
	{
	case Excalibur::VF_None:
		assert(false);
	case Excalibur::VF_P3F:
		glVertexAttribPointer(mIndex, 3, GL_FLOAT, false, 0, 0);
		break;
	case Excalibur::VF_T2F:
		glVertexAttribPointer(mIndex, 2, GL_FLOAT, false, 0, 0);
		break;
	case Excalibur::VF_N3F:
		glVertexAttribPointer(mIndex, 3, GL_FLOAT, false, 0, 0);
		break;
	case Excalibur::VF_C4B:
		glVertexAttribPointer(mIndex, 4, GL_FLOAT, false, 0, 0);
		break;
	default:
		assert(false);
	}
}

void Excalibur::VertexBufferGL::Finialize()  
{
	glDeleteBuffers(1, &mVBO);
}
