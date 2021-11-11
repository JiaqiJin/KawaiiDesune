#pragma once

#include <cstdint>
#include <cstddef>

// http://allenchou.net/2013/05/memory-management-part-1-of-3-the-allocator/

namespace Kawaii::Core
{
    struct BlockHeader
    {
        BlockHeader* next;
    };

    struct PageHeader 
    {
        // followed by blocks in this page
        PageHeader* next;

        // helper function that gives the first block
        BlockHeader* Blocks() { return reinterpret_cast<BlockHeader*>(this + 1); }
    };

    class Allocator 
    {
    public:
        // debug patterns
        static const uint8_t PATTERN_ALIGN = 0XFC;
        static const uint8_t PATTERN_ALLOC = 0XFD;
        static const uint8_t PATTERN_FREE = 0XFE;

        Allocator();
        Allocator(size_t dataSize, size_t pageSize, size_t alignment);
        Allocator(const Allocator* clone);
        Allocator& operator=(const Allocator& rhs);
        ~Allocator();

        // resets the allocator to a new configuration
        void Reset(size_t dataSize, size_t pageSize, size_t alignment);

        // allocates a block of memory
        void* Allocate();
        // deallocates a block of memory
        void  Free(void* p);
        // deallocates all memory
        void  FreeAll();

    private:
        // fill a free page with debug patterns
        void FillFreePage(PageHeader* page);
        // fill a free block with debug patterns
        void FillFreeBlock(BlockHeader* block);
        // fill an allocated block with debug patterns
        void FillAllocatedBlock(BlockHeader* block);

        // Gets Next Blocks
        BlockHeader* NextBlock(BlockHeader* block);
        // the page list
        PageHeader* m_PageList = nullptr;
        // the free list
        BlockHeader* m_FreeList = nullptr;

        // size-related data
        size_t   m_DataSize = 0;
        size_t   m_PageSize = 0;
        size_t   m_AlignmentSize = 0;
        size_t   m_BlockSize = 0;
        uint32_t m_BlocksPerPage = 0;

        // statistics
        uint32_t m_Pages = 0;
        uint32_t m_BLocks = 0;
        uint32_t m_FreeBlocks = 0;
    };
}