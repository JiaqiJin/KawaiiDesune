#pragma once

#include "AllocatorUtil.h"

namespace Util
{
	class LinearAllocator
	{
	public:
		LinearAllocator(OffsetType maxSize, OffsetType reserve = 0) 
			: m_maxSize{ maxSize }, m_reserve{ reserve }, m_top{ reserve } {}
		LinearAllocator(LinearAllocator const&) = default;
		LinearAllocator& operator = (LinearAllocator const&) = delete;
		LinearAllocator(LinearAllocator&&) = default;
		LinearAllocator& operator=(LinearAllocator&&) = delete;
		~LinearAllocator() = default;
		
		OffsetType Allocate(OffsetType size, OffsetType align = 0)
		{
			auto aligned_top = Align(m_top, align);
			if (aligned_top + size >= m_maxSize) return INVALID_OFFSET;
			else
			{
				OffsetType start = aligned_top;
				aligned_top += size;
				m_top = aligned_top;
				return start;
			}
		}

		void Clear()
		{
			m_top = m_reserve;
		}

		OffsetType MaxSize()  const { return m_maxSize; }
		bool Full()			  const { return m_top == m_maxSize; };
		bool Empty()		  const { return m_top == m_reserve; };
		OffsetType UsedSize() const { return m_top; }
	private:
		OffsetType const m_maxSize;
		OffsetType const m_reserve;
		OffsetType m_top;
	};
}