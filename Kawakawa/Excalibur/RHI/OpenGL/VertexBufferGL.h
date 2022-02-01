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

		int GetIndex() { return m_Index; }
		GLuint GetVBO() { return m_VBO; }
	protected:
		int m_Index;
		GLuint m_VBO;
	};
}