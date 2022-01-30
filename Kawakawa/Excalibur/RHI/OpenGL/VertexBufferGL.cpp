#include "VertexBufferGL.h"

#include <cassert>

namespace Excalibur
{
	VertexBufferGL::VertexBufferGL(void* data, unsigned int count, VertexFormat vf, int index) : mIndex(index)
	{
		Initialize(data, count, vf);
	}

	VertexBufferGL::~VertexBufferGL()
	{
		glDeleteBuffers(1, &mVBO);
	}

	void VertexBufferGL::Initialize(void* data, unsigned int count, VertexFormat vf) noexcept
	{
		mVertexCount = count;
		mVertexFormat = vf;
		size_t dataSize = GetVertexSize(vf) * count;
		glGenBuffers(1, &mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
		glEnableVertexAttribArray(mIndex);

		switch (vf)
		{
		case VF_None:
			assert(false);
		case VF_P3F:
			glVertexAttribPointer(mIndex, 3, GL_FLOAT, false, 0, 0);
			break;
		case VF_T2F:
			glVertexAttribPointer(mIndex, 2, GL_FLOAT, false, 0, 0);
			break;
		case VF_N3F:
			glVertexAttribPointer(mIndex, 3, GL_FLOAT, false, 0, 0);
			break;
		case VF_C4B:
			glVertexAttribPointer(mIndex, 4, GL_FLOAT, false, 0, 0);
			break;
		default:
			assert(false);
		}
	}
}