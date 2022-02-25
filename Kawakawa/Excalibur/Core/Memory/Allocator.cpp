#include <stdlib.h>
#include <string.h>
#include "Allocator.h"

namespace Excalibur
{
	Allocator::Allocator()
		: m_pPageList(nullptr), m_pFreeList(nullptr),
		m_szDataSize(0), m_szPageSize(0), m_szAlignmentSize(0),
		m_szBlockSize(0), m_nBlockPerPage(0)
	{
	
	}

	Allocator::Allocator(size_t dataSize, size_t pageSize, size_t alignment) :
		m_pPageList(nullptr), m_pFreeList(nullptr)
	{
		Reset(dataSize, pageSize, alignment);
	}

	Allocator::~Allocator() {
		FreeAll();
	}

	void Allocator::Reset(size_t dataSize, size_t pageSize, size_t alignment)
	{
		FreeAll();

		m_szDataSize = dataSize;
		m_szPageSize = pageSize;
		size_t minBlockSize = (sizeof(PageHeader) > m_szDataSize) ? sizeof(PageHeader) : m_szDataSize;

		m_szBlockSize = ALIGN(minBlockSize, alignment);
		m_szAlignmentSize = m_szBlockSize - minBlockSize;
		m_nBlockPerPage = (m_szPageSize - sizeof(PageHeader)) / m_szBlockSize;
	}

	void* Allocator::Allocate()
	{
		if (!m_pFreeList) {
			PageHeader* pNewPage = reinterpret_cast<PageHeader*>(new uint8_t[m_szPageSize]);
			pNewPage->next = nullptr;
			++m_nPages;
			m_nBlock += m_nBlockPerPage;
			m_nFreeBlocks += m_nBlockPerPage;

			if (m_pPageList) {
				pNewPage->next = m_pPageList;
			}
			m_pPageList = pNewPage;
			BlockHeader* pBlock = pNewPage->GetBlocks();

			for (int i = 0; i < m_nBlockPerPage - 1; ++i) {
				pBlock->next = CalcNextBlock(pBlock);
				pBlock = pBlock->next;
			}
			pBlock->next = nullptr;
			m_pFreeList = pNewPage->GetBlocks();

		}
		BlockHeader* pFreeBlock = m_pFreeList;
		m_pFreeList = m_pFreeList->next;
		--m_nFreeBlocks;

		return reinterpret_cast<void*>(pFreeBlock);
	}

	void Allocator::Free(void* p)
	{
		BlockHeader* block = reinterpret_cast<BlockHeader*>(p);
		block->next = m_pFreeList;
		m_pFreeList = block;
		++m_nFreeBlocks;
	}

	void Allocator::FreeAll()
	{
		PageHeader* pPage = m_pPageList;
		while (pPage)
		{
			PageHeader* p = pPage;
			pPage = pPage->next;
			delete[] reinterpret_cast<uint8_t*>(p);
		}

		m_pPageList = nullptr;
		m_pFreeList = nullptr;

		m_nPages = 0;
		m_nBlock = 0;
		m_nFreeBlocks = 0;
	}

	BlockHeader* Allocator::CalcNextBlock(BlockHeader* pBlock)
	{
		// reinterpret_cast first and then offset m_szBlockSize bytes to get next block address
		uint8_t* address = reinterpret_cast<uint8_t*>(pBlock) + m_szBlockSize;
		return reinterpret_cast<BlockHeader*>(address);
	}

}