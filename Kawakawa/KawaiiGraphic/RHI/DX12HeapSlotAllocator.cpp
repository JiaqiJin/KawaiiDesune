#include "pch.h"
#include "DX12HeapSlotAllocator.h"
#include <assert.h>

namespace RHI
{
	DX12HeapSlotAllocator::DX12HeapSlotAllocator(ID3D12Device* Device, D3D12_DESCRIPTOR_HEAP_TYPE Type, uint32_t NumDescriptorsPerHeap)
		: m_Device(Device),
		m_HeapDesc(CreateHeapDesc(Type, NumDescriptorsPerHeap)),
		m_DescriptorSize(m_Device->GetDescriptorHandleIncrementSize(m_HeapDesc.Type))
	{

	}

	DX12HeapSlotAllocator::~DX12HeapSlotAllocator()
	{
		
	}

	D3D12_DESCRIPTOR_HEAP_DESC DX12HeapSlotAllocator::CreateHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE Type, uint32_t NumDescriptorsPerHeap)
	{
		D3D12_DESCRIPTOR_HEAP_DESC Desc = {};
		Desc.Type = Type;
		Desc.NumDescriptors = NumDescriptorsPerHeap;
		Desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // This heap will not be bound to the shader
		Desc.NodeMask = 0;

		return Desc;
	}

	DX12HeapSlotAllocator::HeapSlot DX12HeapSlotAllocator::AllocateHeapSlot()
	{
		// Find the entry with free list
		int EntryIndex = -1;
		for (int i = 0; i < m_HeapMap.size(); i++)
		{
			if (m_HeapMap[i].FreeList.size() > 0)
			{
				EntryIndex = i;
				break;
			}
		}

		// If all entries are full, create a new one
		if (EntryIndex == -1)
		{
			AllocateHeap();

			EntryIndex = int(m_HeapMap.size() - 1);
		}

		HeapEntry& Entry = m_HeapMap[EntryIndex];
		assert(Entry.FreeList.size() > 0);

		// Allocate  a slot
		FreeRange& Range = Entry.FreeList.front();
		HeapSlot Slot = { (uint32_t)EntryIndex, Range.Start };

		// Remove this range if all slot has been allocated.
		Range.Start += m_DescriptorSize;
		if (Range.Start == Range.End)
		{
			Entry.FreeList.pop_front();
		}

		return Slot;
	}

	void DX12HeapSlotAllocator::FreeHeapSlot(const HeapSlot& Slot)
	{
		assert(Slot.HeapIndex < m_HeapMap.size());
		HeapEntry& Entry = m_HeapMap[Slot.HeapIndex];

		FreeRange NewRange =
		{
			Slot.Handle.ptr,
			Slot.Handle.ptr + m_DescriptorSize
		};

		bool bFound = false;
		for (auto Node = Entry.FreeList.begin(); Node != Entry.FreeList.end() && !bFound; Node++)
		{
			FreeRange& Range = *Node;
			assert(Range.Start < Range.End);

			if (Range.Start == NewRange.End) //Merge NewRange and Range
			{
				Range.Start = NewRange.Start;
				bFound = true;
			}
			else if (Range.End == NewRange.Start) // Merge Range and NewRange
			{
				Range.End = NewRange.End;
				bFound = true;
			}
			else
			{
				assert(Range.End < NewRange.Start || Range.Start > NewRange.Start);
				if (Range.Start > NewRange.Start) // Insert NewRange before Range
				{
					Entry.FreeList.insert(Node, NewRange);
					bFound = true;
				}
			}
		}

		if (!bFound)
		{
			// Add  NewRange to tail
			Entry.FreeList.push_back(NewRange);
		}
	}

	void DX12HeapSlotAllocator::AllocateHeap()
	{
		// Create a new descriptorHeap
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Heap;
		ThrowIfFailed(m_Device->CreateDescriptorHeap(&m_HeapDesc, IID_PPV_ARGS(&Heap)));
		SetDebugName(Heap.Get(), L"DX12HeapSlotAllocator Descriptor Heap");

		// Add an entry covering the entire heap.
		DescriptorHandle HeapBase = Heap->GetCPUDescriptorHandleForHeapStart();
		assert(HeapBase.ptr != 0);

		HeapEntry Entry;
		Entry.Heap = Heap;
		Entry.FreeList.push_back({ HeapBase.ptr, HeapBase.ptr + (SIZE_T)m_HeapDesc.NumDescriptors * m_DescriptorSize });

		// Add the entry to HeapMap
		m_HeapMap.push_back(Entry);
	}
}