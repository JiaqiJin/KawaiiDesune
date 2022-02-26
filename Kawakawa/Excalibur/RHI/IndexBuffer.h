#pragma once
#include "../Interface/IResource.h"

namespace Excalibur {

	class IIndexBuffer : public IRenderResource
	{
	public:
		IIndexBuffer() : m_IndexCount(0), m_IndexFormat(IndexFormat::IF_None) {}
		unsigned int GetIndexCount() { return m_IndexCount; }

		virtual void Initialize(void* data, unsigned int count, IndexFormat iformat) = 0;
		virtual void Finialize() = 0;

	protected :
		unsigned int m_IndexCount;
		IndexFormat	m_IndexFormat;
	};

}