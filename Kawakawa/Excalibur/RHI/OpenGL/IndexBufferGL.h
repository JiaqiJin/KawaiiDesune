#pragma once

#include "../IndexBufferBase.h"
#include <glad/glad.h>

namespace Excalibur
{
	class IndexBufferGL : public IndexBufferBase
	{
	public:
		IndexBufferGL(void* data, uint32_t count);
		virtual ~IndexBufferGL();
		virtual void Initialize(void* data, unsigned int count) final;

	protected:
		GLuint mEBO;
	};
}