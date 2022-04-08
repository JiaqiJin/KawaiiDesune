#pragma once

#include "DX12Resource.h"
#include <stdint.h>
#include <set>

#define DEFAULT_POOL_SIZE (512 * 1024 * 512)

#define DEFAULT_RESOURCE_ALIGNMENT 4
#define UPLOAD_RESOURCE_ALIGNMENT 256

/*
* BuddyAllocator :
* - When allocating, if the free memory block is too large, it will be continuously divided into two parts
*  and the divided memory will be smaller than the required space.
* - When releasing, it will detect whether Buddy is free, and if so, it will be merged with Buddy into a large free block. 
* If the merged large free block finds that its Buddy is also free, it will continue to merge until it cannot be merged.
* https://github.com/EpicGames/UnrealEngine/blob/c3caf7b6bf12ae4c8e09b606f10a09776b4d1f38/Engine/Source/Runtime/D3D12RHI/Private/D3D12Allocation.h
*/

namespace RHI
{
	class DX12ResourceAllocation;
	struct DX12BuddyBlockData;

	class DX12BuddyAllocator
	{
	public:
		enum class EAllocationStrategy
		{
			// This strategy uses Placed Resources to sub-allocate a buffer out of an underlying ID3D12Heap.
			// The benefit of this is that each buffer can have it's own resource state and can be treated
			// as any other buffer. The downside of this strategy is the API limitation which enforces
			// the minimum buffer size to 64k leading to large internal fragmentation in the allocator
			kPlacedResource,
			// The alternative is to manually sub-allocate out of a single large buffer which allows block
			// allocation granularity down to 1 byte. However, this strategy is only really valid for buffers which
			// will be treated as read-only after their creation (i.e. most Index and Vertex buffers). This 
			// is because the underlying resource can only have one state at a time.
			kManualSubAllocation
		};

		struct AllocatorInitData
		{
			EAllocationStrategy AllocationStrategy;  // Allocation Strategy
			D3D12_HEAP_TYPE HeapType; 
			D3D12_HEAP_FLAGS HeapFlags = D3D12_HEAP_FLAG_NONE;  // Only for PlacedResource
			D3D12_RESOURCE_FLAGS ResourceFlags = D3D12_RESOURCE_FLAG_NONE;  // Only for ManualSubAllocation
		};

	public:
		DX12BuddyAllocator(ID3D12Device* Device, const AllocatorInitData& initData);
		~DX12BuddyAllocator();

		bool AllocResource(uint32_t Size, uint32_t Alignment, DX12ResourceAllocation& ResourceLocation);

		void Deallocate(DX12ResourceAllocation& ResourceLocation);

		void CleanUpAllocations();

		ID3D12Heap* GetBackingHeap() { return BackingHeap; }

		EAllocationStrategy GetAllocationStrategy() { return InitData.AllocationStrategy; }
	private:
		void Initialize();

		// Find a suitable free block and allocate it.
		uint32_t AllocateBlock(uint32_t Order);

		uint32_t GetSizeToAllocate(uint32_t Size, uint32_t Alignment);

		// Check to see if there is more space to allocate
		bool CanAllocate(uint32_t SizeToAllocate);

		void DeallocateInternal(const DX12BuddyBlockData& Block);

		// When the ResourceAllocation is destroyed, the video memory resources are released.
		// Instead of releasing immediately, the block information is saved in the destruction queue
		void DeallocateBlock(uint32_t Offset, uint32_t Order);

		uint32_t GetBuddyOffset(const uint32_t& Offset, const uint32_t& Size)
		{
			return Offset ^ Size;
		}

		uint32_t GetAllocOffsetInBytes(uint32_t Offset) const { return Offset * MinBlockSize; }

		uint32_t SizeToUnitSize(uint32_t Size) const
		{
			return (Size + (MinBlockSize - 1)) / MinBlockSize;
		}

		uint32_t UnitSizeToOrder(uint32_t Size) const
		{
			unsigned long Result;
			_BitScanReverse(&Result, Size + Size - 1); // ceil(log2(size))
			return Result;
		}

		uint32_t OrderToUnitSize(uint32_t Order) const
		{
			return ((uint32_t)1) << Order;
		}

	private:
		// Allocator Init Data (Heap datas, type)
		AllocatorInitData InitData;
		// Min Block Size
		const uint32_t MinBlockSize = 256;
		// The level corresponding to the required size
		uint32_t MaxOrder;
		uint32_t TotalAllocSize = 0;
		std::vector<std::set<uint32_t>> FreeBlocks;
		std::vector<DX12BuddyBlockData> DeferredDeletionQueue;

		ID3D12Device* m_Device;
		// Backing Resource
		DX12Resource* BackingResource = nullptr;
		// Backing Heap
		ID3D12Heap* BackingHeap = nullptr;
	};

	// Contains a set of BuddyAllocators, When applying for resources, 
	// if it is found that the existing BuddyAllocator has no free space, a new BuddyAllocator will be created for allocation.
	class DX12MultiBuddyAllocator
	{
	public:
		DX12MultiBuddyAllocator(ID3D12Device* InDevice, const DX12BuddyAllocator::AllocatorInitData& InInitData);

		~DX12MultiBuddyAllocator();

		bool AllocResource(uint32_t Size, uint32_t Alignment, DX12ResourceAllocation& ResourceLocation);

		void CleanUpAllocations();
	private:
		std::vector<std::shared_ptr<DX12BuddyAllocator>> m_Allocators;
		ID3D12Device* m_Device;
		DX12BuddyAllocator::AllocatorInitData m_InitData;
	};

	// DX12UploadBufferAllocator
	class DX12UploadBufferAllocator
	{
	public:
		DX12UploadBufferAllocator(ID3D12Device* InDevice);

		void* AllocUploadResource(uint32_t Size, uint32_t Alignment, DX12ResourceAllocation& ResourceLocation);

		void CleanUpAllocations();

	private:
		std::unique_ptr<DX12MultiBuddyAllocator> Allocator = nullptr;

		ID3D12Device* D3DDevice = nullptr;
	};

	// DX12DefaultBufferAllocator
	class DX12DefaultBufferAllocator
	{
	public:
		DX12DefaultBufferAllocator(ID3D12Device* InDevice);

		void AllocDefaultResource(const D3D12_RESOURCE_DESC& ResourceDesc, uint32_t Alignment, DX12ResourceAllocation& ResourceLocation);

		void CleanUpAllocations();

	private:
		std::unique_ptr<DX12MultiBuddyAllocator> Allocator = nullptr;

		std::unique_ptr<DX12MultiBuddyAllocator> UavAllocator = nullptr;

		ID3D12Device* D3DDevice = nullptr;
	};

	// DX12TextureResourceAllocator
	class DX12TextureResourceAllocator
	{
	public:
		DX12TextureResourceAllocator(ID3D12Device* InDevice);

		void AllocTextureResource(const D3D12_RESOURCE_STATES& ResourceState,
			const D3D12_RESOURCE_DESC& ResourceDesc, DX12ResourceAllocation& ResourceLocation);

		void CleanUpAllocations();

	private:
		std::unique_ptr<DX12MultiBuddyAllocator> Allocator = nullptr;

		ID3D12Device* D3DDevice = nullptr;
	};
}