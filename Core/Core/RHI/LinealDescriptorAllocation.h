#pragma once

#include "DescriptorHeap.h"
#include "../Utils/LinearAllocator.h"

using namespace Util;

namespace RHI
{
	class LinealDescriptorAllocation : public DescriptorHeap
	{
	public:
		// Constructors
		LinealDescriptorAllocation(ID3D12DescriptorHeap* pExistingHeap, OffsetType reserve = 0);

		LinealDescriptorAllocation(
			ID3D12Device* device,
			D3D12_DESCRIPTOR_HEAP_DESC const& desc,
			OffsetType reserve = 0);

		LinealDescriptorAllocation(
			ID3D12Device* device,
			D3D12_DESCRIPTOR_HEAP_TYPE type,
			D3D12_DESCRIPTOR_HEAP_FLAGS flags,
			size_t capacity, OffsetType reserve = 0);

		LinealDescriptorAllocation(ID3D12Device* device, size_t count, OffsetType reserve = 0);

		~LinealDescriptorAllocation() = default;

		OffsetType Allocate();
		OffsetType AllocateRange(size_t);
		void Clear();

	private:
		// Lineal Allocation
		LinearAllocator m_linearAllocator;
		// Mutex
		std::mutex m_mutex;
	};
}