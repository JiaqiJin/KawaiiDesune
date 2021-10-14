#include "../Kawaiipch.h"

#include "LinealDescriptorAllocation.h"

using namespace Util;

namespace RHI
{
	LinealDescriptorAllocation::LinealDescriptorAllocation(ID3D12DescriptorHeap* pExistingHeap, OffsetType reserve)
		: DescriptorHeap(pExistingHeap), m_linearAllocator(Count(), reserve)

	{

	}

	LinealDescriptorAllocation::LinealDescriptorAllocation(ID3D12Device* device, 
		D3D12_DESCRIPTOR_HEAP_DESC const& desc, OffsetType reserve)
		: DescriptorHeap(device, desc), m_linearAllocator(Count(), reserve)
	{

	}

	LinealDescriptorAllocation::LinealDescriptorAllocation(ID3D12Device* device, 
		D3D12_DESCRIPTOR_HEAP_TYPE type, 
		D3D12_DESCRIPTOR_HEAP_FLAGS flags, 
		size_t capacity, OffsetType reserve)
		: DescriptorHeap(device, type, flags, capacity), m_linearAllocator(Count(), reserve)
	{

	}

	LinealDescriptorAllocation::LinealDescriptorAllocation(ID3D12Device* device, size_t count, OffsetType reserve) :
		LinealDescriptorAllocation(device, 
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 
			D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 
			count, reserve)
	{

	}

	OffsetType LinealDescriptorAllocation::Allocate()
	{
		return AllocateRange(1);
	}

	OffsetType LinealDescriptorAllocation::AllocateRange(size_t num_descriptors)
	{
		OffsetType start = INVALID_OFFSET;
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			start = m_linearAllocator.Allocate(num_descriptors);
		}
		assert(start != INVALID_OFFSET && "Don't have enough space");

		return start;
	}

	void LinealDescriptorAllocation::Clear()
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		m_linearAllocator.Clear();
	}
}