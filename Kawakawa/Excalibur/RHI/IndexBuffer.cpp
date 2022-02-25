#include "IndexBuffer.h"

namespace Excalibur
{
	void IndexBuffer::Initialize(void* data, unsigned int count, IndexFormat iformat)
	{
		m_Count = count;
		m_IndexFormat = iformat;
	}

	void IndexBuffer::Finialize()
	{
	}
}
