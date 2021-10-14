#pragma once

#include "AllocatorUtil.h"

using namespace Util;

// Class for linear allocation
class LinearAllocator
{
public:
	// Constructor
	LinearAllocator(OffsetType maxSize, OffsetType reserve = 0)
		: m_maxSize{ maxSize }, m_reserve{ reserve }, m_top{ reserve } {}
	LinearAllocator(LinearAllocator const&) = default;
	LinearAllocator& operator = (LinearAllocator const&) = delete;
	LinearAllocator(LinearAllocator&&) = default;
	LinearAllocator& operator=(LinearAllocator&&) = delete;
	~LinearAllocator() = default;

	OffsetType Allocate(OffsetType size, OffsetType align = 0)
	{
		// Align the top 
		auto aligned_top = Align(m_top, align);
		// If the size of the aligned top > max size return invalid offset
		if (aligned_top + size >= m_maxSize) return INVALID_OFFSET;
		// Return the aligned top and increment top offset
		else
		{
			OffsetType start = aligned_top;
			aligned_top += size;
			m_top = aligned_top;
			return start;
		}
	}

	// Clear the top 
	void Clear()
	{
		m_top = m_reserve;
	}

	OffsetType MaxSize()  const { return m_maxSize; }
	bool Full()			  const { return m_top == m_maxSize; };
	bool Empty()		  const { return m_top == m_reserve; };
	OffsetType UsedSize() const { return m_top; }
private:
	// Max size of allocator
	OffsetType const m_maxSize; 
	// Reserver size
	OffsetType const m_reserve;
	// Allcotor Top
	OffsetType m_top;
};