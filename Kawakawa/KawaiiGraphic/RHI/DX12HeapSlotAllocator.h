#pragma once

namespace RHI
{
	// Responsible for creating non-shader-visible descriptor heaps, and allocating slots for creating descriptors.
	class DX12HeapSlotAllocator
	{
	public:
		typedef D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle;
		typedef decltype(DescriptorHandle::ptr) DescriptorHandleRaw;

		struct HeapSlot
		{
			uint32_t HeapIndex;
			D3D12_CPU_DESCRIPTOR_HANDLE Handle;
		};

	private:
		struct FreeRange
		{
			DescriptorHandleRaw Start;
			DescriptorHandleRaw End;
		};

		struct HeapEntry
		{
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Heap = nullptr;
			std::list<DX12HeapSlotAllocator::FreeRange> FreeList;

			HeapEntry() { }
		};

	public:
		DX12HeapSlotAllocator(ID3D12Device* Device, D3D12_DESCRIPTOR_HEAP_TYPE Type, uint32_t NumDescriptorsPerHeap);

		~DX12HeapSlotAllocator();
		// Whenever a rendering resource needs to create a descriptor, the HeapSlotAllocator will allocate a free slot
		HeapSlot AllocateHeapSlot();
		// If the freed slot can be merged with the existing free space, if not, add a new free space.
		void FreeHeapSlot(const HeapSlot& Slot);
	private:
		// Create Descriptor Heap (with flag D3D12_DESCRIPTOR_HEAP_FLAG_NONE), The entire descriptor heap is free
		D3D12_DESCRIPTOR_HEAP_DESC CreateHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE Type, uint32_t NumDescriptorsPerHeap);

		void AllocateHeap();

	private:
		ID3D12Device* m_Device;
		// Descriptor Desc
		const D3D12_DESCRIPTOR_HEAP_DESC m_HeapDesc;
		// Descriptor Size
		const uint32_t m_DescriptorSize;
		// Heap Entry (Heap, Free List(Start, End))
		std::vector<HeapEntry> m_HeapMap;
	};
}