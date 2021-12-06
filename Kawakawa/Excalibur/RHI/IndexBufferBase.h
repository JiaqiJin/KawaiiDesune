#pragma once

#include "../Interface/IResourceh.h"

namespace Excalibur
{
	class IndexBufferBase : public IResouceBase
	{
	public:
		IndexBufferBase() : m_IndexCount(0) {}
		virtual void Initialize(void* data, unsigned int count) = 0;
	protected:
		uint32_t m_IndexCount;
	};
}