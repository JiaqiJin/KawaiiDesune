#include "DescriptorAllocator.h"

namespace Kawaii::Graphics::backend::DX12
{
	DescriptorAllocation::DescriptorAllocation(D3D12_CPU_DESCRIPTOR_HANDLE handle, std::shared_ptr<DescriptorPage> page)
		: m_Handle(std::move(handle)), m_Page(std::move(page))
	{

	}

	DescriptorAllocation::~DescriptorAllocation()
	{
		if (m_Page)
			m_Page->DiscardDescriptor(*this);
	}

	// ---------------------------- Descriptor Page ------------------------------
	void DescriptorPage::DiscardDescriptor(DescriptorAllocation& descriptor)
	{
		auto offset = (descriptor.m_Handle.ptr - m_Handle.ptr) / m_IncrementSize;
		size_t size = 1;

		// TODO
	}

	std::optional<std::vector<DescriptorAllocation>> DescriptorPage::Allocate(size_t num_descriptors)
	{
		std::vector<DescriptorAllocation> allocation;

		return allocation;
	}

	// ---------------------------- Descriptor Allocator -------------------------
	DescriptorAllocator::DescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptorsPerHeap = 1024)
		: m_Type(type), m_NumDescriptorPerPage(numDescriptorsPerHeap)
	{

	}

	void DescriptorAllocator::Initialize(ID3D12Device* device)
	{
		assert(device);
		m_Device = device;
	}

	DescriptorAllocation DescriptorAllocator::Allocate()
	{
		auto ret = Allocate(1);
		return std::move(ret[0]);
	}

	std::vector<DescriptorAllocation> DescriptorAllocator::Allocate(size_t numDescriptors)
	{
		// if the size larget than current page size, update default page size and create large page
		m_NumDescriptorPerPage = std::max(m_NumDescriptorPerPage, numDescriptors);

		std::vector<DescriptorAllocation> allocation;

		for (auto&& page : m_PagePool)
		{
			if (auto descriptorAlloc = page->Allocate(numDescriptors))
				allocation = std::move(descriptorAlloc.value());
		}

		if (allocation.empty())
		{
			// New Page
			// TODO
		}

		return allocation;
	}
}