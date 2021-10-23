#pragma once

#include <queue>
#include <cassert>
#include "AllocatorUtil.h"

namespace Util
{
	class RingAllocator
	{
	public:
		struct BufferAttribute
		{
			BufferAttribute(uint64_t fv, OffsetType off, OffsetType sz) :
				FenceValue(fv),
				Offset(off),
				Size(sz)
			{}
			// Fence value associated with the command list in which 
			// the allocation could have been referenced last time
			uint64_t FenceValue;
			OffsetType Offset;
			OffsetType Size;
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

		OffsetType Allocate(OffsetType size, OffsetType align = 0)
		{
			if (Full())
			{
				return INVALID_OFFSET;
			}

			if (m_tail > m_head)
			{
				//                     Head             Tail     MaxSize
				//                     |                |        |
				//  [                  xxxxxxxxxxxxxxxxx         ]
				if (m_tail + size <= m_maxSize)
				{
					auto offset = m_tail;
					m_tail += size;
					m_usedSize += size;
					m_currentFrameSize += size;
					return offset + m_reserved;
				}
				else if (size <= m_head)
				{
					// Allocate from the beginning of the buffer
					OffsetType AddSize = (m_maxSize - m_tail) + size;
					m_usedSize += AddSize;
					m_currentFrameSize += AddSize;
					m_tail = size;
					return 0 + m_reserved;;
				}
			}
			else if(m_tail + size <= m_head)
			{
				//
				//       Tail          Head             
				//       |             |             
				//  [xxxx              xxxxxxxxxxxxxxxxxxxxxxxxxx]
				auto offset = m_tail;
				m_tail += size;
				m_usedSize += size;
				m_currentFrameSize += size;
				return offset + m_reserved;
			}

			return INVALID_OFFSET;
		}

		void FinishCurrentFrame(uint64_t frame)
		{
			m_completeFrame.emplace(frame, m_tail, m_currentFrameSize);
			m_currentFrameSize = 0;
		}

		void ReleaseCompletedFrames(uint64_t CompletedFenceValue)
		{
			// We can release all tails whose associated fence value is less than or equal to CompletedFenceValue
			while (!m_completeFrame.empty() && m_completeFrame.front().FenceValue <= CompletedFenceValue)
			{
				auto const& oldestFrameTail = m_completeFrame.front();
				assert(oldestFrameTail.Size <= m_usedSize);
				m_usedSize -= oldestFrameTail.Size;
				m_head = oldestFrameTail.Offset;
				m_completeFrame.pop();
			}
		}

		OffsetType MaxSize()  const { return m_maxSize; }
		bool Full()			  const { return m_usedSize == m_maxSize; };
		bool Empty()		  const { return m_usedSize == m_reserved; };
		OffsetType UsedSize() const { return m_usedSize; }
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