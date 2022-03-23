#pragma once

namespace RHI
{
	class DX12HeapAllocator
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
			std::list<DX12HeapAllocator::FreeRange> FreeList;

			HeapEntry() { }
		};

	public:
		DX12HeapAllocator(ID3D12Device* Device, D3D12_DESCRIPTOR_HEAP_TYPE Type, uint32_t NumDescriptorsPerHeap);

		~DX12HeapAllocator();

		HeapSlot AllocateHeapSlot();

		void FreeHeapSlot(const HeapSlot& Slot);
	private:
		D3D12_DESCRIPTOR_HEAP_DESC CreateHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE Type, uint32_t NumDescriptorsPerHeap);

		void AllocateHeap();

	private:
		ID3D12Device* m_Device;

		const D3D12_DESCRIPTOR_HEAP_DESC m_HeapDesc;

		const uint32_t m_DescriptorSize;

		std::vector<HeapEntry> m_HeapMap;
	};
}