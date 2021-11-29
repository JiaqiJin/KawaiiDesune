#include "DescriptorAllocator.h"

namespace Kawaii::Graphics::backend::DX12
{
	Descriptor::Descriptor(D3D12_CPU_DESCRIPTOR_HANDLE handle, std::shared_ptr<DescriptorPage> page)
		: m_Handle(std::move(handle)), m_Page(std::move(page))
	{

	}

	Descriptor ::~Descriptor()
	{
		if (m_Page)
			m_Page->DiscardDescriptor(*this);
	}

	// ---------------------------- Descriptor Page ------------------------------

	DescriptorPage::DescriptorPage(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, size_t num_descriptors)
	{
		assert(device);
		m_IncrementSize = device->GetDescriptorHandleIncrementSize(type);

		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;  // Just CPU Visiable
		desc.NodeMask = 0;
		desc.NumDescriptors = num_descriptors;
		desc.Type = type;

		ThrowIfFailed(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_DescriptorHeap)));
		m_DescriptorHeap->SetName(L"CPU Descriptor Heap");
		m_Handle = m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart();

		// TODO
	}

	void DescriptorPage::DiscardDescriptor(Descriptor& descriptor)
	{
		auto offset = (descriptor.m_Handle.ptr - m_Handle.ptr) / m_IncrementSize;
		size_t size = 1;

		// TODO
	}

	std::optional<std::vector<Descriptor>> DescriptorPage::Allocate(size_t num_descriptors)
	{
		std::vector<Descriptor> allocation;

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

	Descriptor  DescriptorAllocator::Allocate()
	{
		auto ret = Allocate(1);
		return std::move(ret[0]);
	}

	std::vector<Descriptor> DescriptorAllocator::Allocate(size_t numDescriptors)
	{
		// if the size larget than current page size, update default page size and create large page
		m_NumDescriptorPerPage = std::max(m_NumDescriptorPerPage, numDescriptors);

		std::vector<Descriptor> allocation;

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