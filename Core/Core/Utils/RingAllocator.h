#pragma once

#include <queue>
#include "AllocatorUtil.h"

namespace Util
{
	class RingAllocator
	{
	public:
		struct BufferAttribute
		{

		};

	public:
		RingAllocator(OffsetType maxSize, OffsetType reserve = 0) noexcept :
			m_completeFrame{},
			m_reserved{ reserve },
			m_maxSize{ maxSize - reserve },
			m_head{ 0 },
			m_tail{ 0 },
			m_usedSize{ reserve }
		{}
		RingAllocator(RingAllocator&& rhs) noexcept :
			m_completeFrame{ std::move(rhs.m_completeFrame) },
			m_reserved{ rhs.m_reserved },
			m_tail{ rhs.m_tail },
			m_head{ rhs.m_head },
			m_maxSize{ rhs.m_maxSize },
			m_usedSize{ rhs.m_usedSize },
			m_currentFrameSize{ rhs.m_currentFrameSize }
		{
			rhs.m_reserved = 0;
			rhs.m_tail = 0;
			rhs.m_head = 0;
			rhs.m_maxSize = 0;
			rhs.m_usedSize = 0;
			rhs.m_currentFrameSize = 0;
		}

		RingAllocator& operator=(RingAllocator&& rhs) noexcept
		{
			m_completeFrame = std::move(rhs.m_completeFrame);
			m_tail = rhs.m_tail;
			m_head = rhs.m_head;
			m_maxSize = rhs.m_maxSize;
			m_usedSize = rhs.m_usedSize;
			m_reserved = rhs.m_reserved;
			m_currentFrameSize = rhs.m_currentFrameSize;

			rhs.m_reserved = 0;
			rhs.m_maxSize = 0;
			rhs.m_tail = 0;
			rhs.m_head = 0;
			rhs.m_usedSize = 0;
			rhs.m_currentFrameSize = 0;

			return *this;
		}
		RingAllocator(RingAllocator const&) = delete;
		RingAllocator& operator = (RingAllocator const&) = delete;
		~RingAllocator() = default;

	private:
		std::queue<BufferAttribute> m_completeFrame;
		OffsetType m_head = 0;
		OffsetType m_tail = 0;
		OffsetType m_reserved = 0;
		OffsetType m_maxSize = 0;
		OffsetType m_usedSize = 0;
		OffsetType m_currentFrameSize = 0;
	};
}