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
		virtual void Initialize(void* data, unsigned int count, IndexFormat iformat) noexcept;
	public:
		GLuint mVEO;
	};
}