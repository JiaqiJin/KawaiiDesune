#pragma once
#include "D3Dpch.h"

#include <optional>

/*
* the DescriptorAllocator is CPU visiable only, for those used shader visiable descriptor,
* use the dynamic descriptor heap (in context).
* CPU visible descriptors are useful for “staging” resource descriptors in CPU memory 
* and later copied to a GPU visible descriptor heap for use in a shader.
*/
// https://www.3dgep.com/learning-directx-12-3/#Descriptor_Allocator

namespace Kawaii::Graphics::backend::DX12
{
	class DescriptorPage;

	// Allocation that is returned from the DescriptorAllocator::Allocate
	// Stores a pointer back to the page it came from and will automatically free itself if the descriptor(s) are no longer required.
	struct DescriptorAllocation
	{
		DescriptorAllocation(D3D12_CPU_DESCRIPTOR_HANDLE handle, std::shared_ptr<DescriptorPage> page);
		DescriptorAllocation(const DescriptorAllocation& rhs) : m_Handle(rhs.m_Handle), m_Page(nullptr) {}
		DescriptorAllocation& operator=(const DescriptorAllocation& rhs)
		{
			if (this != &rhs) m_Handle = rhs.m_Handle;
			return *this;
		}

		DescriptorAllocation(DescriptorAllocation&&) = default;
		DescriptorAllocation& operator=(DescriptorAllocation&&) = default;
		~DescriptorAllocation();

		// The base descriptor.
		D3D12_CPU_DESCRIPTOR_HANDLE m_Handle;
		// A pointer back to the original page where this allocation came from.
		std::shared_ptr<DescriptorPage> m_Page;
	};

	// The AllocatorPage also keeps track of the free list for the Descriptor.
	class DescriptorPage : public std::enable_shared_from_this<DescriptorPage>
	{
	public:

		std::optional<std::vector<DescriptorAllocation>> Allocate(size_t num_descriptors);
		void DiscardDescriptor(DescriptorAllocation& descriptor);
	private:
		ComPtr<ID3D12DescriptorHeap> m_DescriptorHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE m_Handle{};
		size_t m_IncrementSize;
	};

	// This is the main interface to the application for requesting descriptors. The DescriptorAllocator class manages the descriptor pages.
	class DescriptorAllocator
	{
	public:
		DescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptorsPerHeap = 1024);

		void Initialize(ID3D12Device* device);

		// Allocate a number of contiguous descriptors from a CPU visible descriptor heap.
		DescriptorAllocation Allocate();
		std::vector<DescriptorAllocation> Allocate(size_t numDescriptor);
	private:
		// Device
		ID3D12Device* m_Device{};
		// Descriptor Type
		D3D12_DESCRIPTOR_HEAP_TYPE  m_Type;
		// Num per Page
		size_t m_NumDescriptorPerPage;
		//
		std::list<std::shared_ptr<DescriptorPage>> m_PagePool;
	};
}