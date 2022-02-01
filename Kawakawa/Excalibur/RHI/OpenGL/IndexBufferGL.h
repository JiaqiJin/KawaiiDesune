#pragma once

#include "../IndexBufferBase.h"
#include <glad/glad.h>

namespace Excalibur
{
	class IndexBufferGL : public IIndexBuffer
	{
	public:
		IndexBufferGL(void* data, unsigned int count, IndexFormat iformat);
		virtual ~IndexBufferGL();
		virtual void Initialize(void* data, unsigned int count, IndexFormat iformat);
		virtual void Finialize();

		GLuint GetVEO() { return m_VEO; }
	protected:
		GLuint m_VEO;
	};
}