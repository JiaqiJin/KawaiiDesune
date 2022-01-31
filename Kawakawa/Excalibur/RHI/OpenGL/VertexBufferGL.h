#pragma once

#include "../VertexBufferBase.h"
#include <glad/glad.h>

namespace Excalibur
{
	class VertexBufferGL : public IVertexBuffer
	{
	public:
		VertexBufferGL(void* data, unsigned int count, VertexFormat vf, int index);
		virtual ~VertexBufferGL();
		virtual void Initialize(void* data, unsigned int count, VertexFormat vf);
		virtual void Finialize();

	public:
		int mIndex;
		GLuint mVBO;
	};
}