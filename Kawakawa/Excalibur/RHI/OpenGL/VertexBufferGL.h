#pragma once

#include "../VertexBufferBase.h"
#include <glad/glad.h>

namespace Excalibur
{
	class VertexBufferGL : public VertexBufferBase
	{
	public:
		VertexBufferGL(void* data, uint32_t count, VertexFormat vf, int index);
		virtual ~VertexBufferGL();
		virtual void Initialize(void* data, uint32_t count, VertexFormat vf) final;
	protected:
		int m_Index;
		GLuint mVBO;
	};
}