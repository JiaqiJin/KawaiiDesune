#pragma once

#include "../Interface/IResourceh.h"

namespace Excalibur
{
	class IIndexBuffer : public IResouceBase
	{
	public:
		virtual void Initialize(void* data, unsigned int count, IndexFormat iformat)
		{
			m_Count = count;
			m_IndexFormat = iformat;
		}

		virtual void Finialize() = 0;

		unsigned int GetCount() { return m_Count; }
		IndexFormat	GetIndexFormat() { return m_IndexFormat; }
	protected:
		unsigned int	m_Count;
		IndexFormat		m_IndexFormat;
	};

}