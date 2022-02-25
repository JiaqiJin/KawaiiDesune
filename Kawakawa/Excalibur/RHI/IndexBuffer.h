#pragma once
#include "../Interface/IResource.h"

namespace Excalibur {

	class IndexBuffer : public IRenderResource
	{
	public:
		virtual void Initialize(void* data, unsigned int count, IndexFormat iformat);
		virtual void Finialize();

	public:
		unsigned int m_Count;
		IndexFormat	m_IndexFormat;
	};

}