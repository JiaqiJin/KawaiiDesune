#include "pch.h"
#include "DX12MemoryAllocator.h"

namespace RHI
{
	DX12BuddyAllocator::DX12BuddyAllocator(ID3D12Device* Device, const AllocatorInitData& initData)
		: m_Device(Device), InitData(initData)
	{
		Initialize();
	}

	DX12BuddyAllocator::~DX12BuddyAllocator()
	{
		if (BackingResource)
		{
			delete BackingResource;
		}

		if (BackingHeap)
		{
			BackingHeap->Release();
		}
	}

	void DX12BuddyAllocator::Initialize()
	{
		// when the allocation strategy is PlacedResource, we need to create a Heap
		if (InitData.AllocationStrategy == EAllocationStrategy::kPlacedResource)
		{
			CD3DX12_HEAP_PROPERTIES HeapProperties(InitData.HeapType);
			D3D12_HEAP_DESC Desc = {};
			Desc.SizeInBytes = DEFAULT_POOL_SIZE;
			Desc.Properties = HeapProperties;
			Desc.Alignment = 0;
			Desc.Flags = InitData.HeapFlags;

			// Create BackingHeap, we will create place resources on it.
			ID3D12Heap* Heap = nullptr;
			ThrowIfFailed(m_Device->CreateHeap(&Desc, IID_PPV_ARGS(&Heap)));
			Heap->SetName(L"DX12BuddyAllocator BackingHeap");

			BackingHeap = Heap;
		}
		else // ManualSubAllocation we are going to create a CommittedResource
		{
			CD3DX12_HEAP_PROPERTIES HeapProperties(InitData.HeapType);
			D3D12_RESOURCE_STATES HeapResourceStates;
			if (InitData.HeapType == D3D12_HEAP_TYPE_UPLOAD)
			{
				HeapResourceStates = D3D12_RESOURCE_STATE_GENERIC_READ;
			}
			else //D3D12_HEAP_TYPE_DEFAULT
			{
				HeapResourceStates = D3D12_RESOURCE_STATE_COMMON;
			}

			CD3DX12_RESOURCE_DESC BufferDesc = CD3DX12_RESOURCE_DESC::Buffer(DEFAULT_POOL_SIZE, InitData.ResourceFlags);

			// Create committed resource, we will allocate sub regions on it.
			Microsoft::WRL::ComPtr<ID3D12Resource> Resource;
			ThrowIfFailed(m_Device->CreateCommittedResource(
				&HeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&BufferDesc,
				HeapResourceStates,
				nullptr,
				IID_PPV_ARGS(&Resource)));

			Resource->SetName(L"DX12BuddyAllocator BackingResource");

			BackingResource = new DX12Resource(Resource);

			if (InitData.HeapType == D3D12_HEAP_TYPE_UPLOAD)
			{
				BackingResource->Map();
			}
		}

		// Then we add a free block that includes the entire memory space we allocated.

		// Initialize free blocks, add the free block for MaxOrder
		MaxOrder = UnitSizeToOrder(SizeToUnitSize(DEFAULT_POOL_SIZE));

		for (uint32_t i = 0; i <= MaxOrder; i++)
		{
			FreeBlocks.emplace_back(std::set<uint32_t>());
		}

		FreeBlocks[MaxOrder].insert((uint32_t)0);
	}

	bool DX12BuddyAllocator::AllocResource(uint32_t Size, uint32_t Alignment, DX12ResourceAllocation& ResourceLocation)
	{
		// If the alignment doesn't match the block size
		uint32_t SizeToAllocate = GetSizeToAllocate(Size, Alignment);

		if (CanAllocate(SizeToAllocate))
		{
			// Work out what size block is needed and allocate one
			const uint32_t UnitSize = SizeToUnitSize(SizeToAllocate);
			const uint32_t Order = UnitSizeToOrder(UnitSize);
			const uint32_t Offset = AllocateBlock(Order); // This is the offset in MinBlockSize units

			const uint32_t AllocSize = UnitSize * MinBlockSize;
			TotalAllocSize += AllocSize;

			//Calculate AlignedOffsetFromResourceBase
			const uint32_t OffsetFromBaseOfResource = GetAllocOffsetInBytes(Offset);
			uint32_t AlignedOffsetFromResourceBase = OffsetFromBaseOfResource;

			if (Alignment != 0 && OffsetFromBaseOfResource % Alignment != 0)
			{
				AlignedOffsetFromResourceBase = AlignArbitrary(OffsetFromBaseOfResource, Alignment);

				uint32_t Padding = AlignedOffsetFromResourceBase - OffsetFromBaseOfResource;
				assert((Padding + Size) <= AllocSize);
			}
			assert((AlignedOffsetFromResourceBase % Alignment) == 0);

			// Save allocation info to ResourceLocation
			ResourceLocation.SetType(DX12ResourceAllocation::EResourceLocationType::SubAllocation);
			ResourceLocation.m_BlockData.Order = Order;
			ResourceLocation.m_BlockData.Offset = Offset;
			ResourceLocation.m_BlockData.CurrentUsedSize = Size;
			ResourceLocation.m_Allocator = this;

			if (InitData.AllocationStrategy == EAllocationStrategy::kManualSubAllocation)
			{
				ResourceLocation.m_UnderlyingResource = BackingResource;
				ResourceLocation.OffsetFromBaseOfResource = AlignedOffsetFromResourceBase;
				ResourceLocation.m_GPUVirtualAddress = BackingResource->m_GPUVirtualAddress + AlignedOffsetFromResourceBase;

				if (InitData.HeapType == D3D12_HEAP_TYPE_UPLOAD)
				{
					ResourceLocation.m_MappedAddress = ((uint8_t*)BackingResource->m_MappedAdderessMemory + AlignedOffsetFromResourceBase);
				}
			}
			else
			{
				ResourceLocation.OffsetFromBaseOfHeap = AlignedOffsetFromResourceBase;

				// Place resource are initialized by caller
			}

			return true;
		}
		else
		{
			return false;
		}
	}

	void DX12BuddyAllocator::Deallocate(DX12ResourceAllocation& ResourceLocation)
	{
		DeferredDeletionQueue.push_back(ResourceLocation.m_BlockData);
	}

	void DX12BuddyAllocator::CleanUpAllocations()
	{
		for (int32_t i = 0; i < DeferredDeletionQueue.size(); i++)
		{
			const DX12BuddyBlockData& Block = DeferredDeletionQueue[i];

			DeallocateInternal(Block);
		}

		// clear out all of the released blocks, don't allow the array to shrink
		DeferredDeletionQueue.clear();
	}

	// For example, if 10KB of space is required, then the corresponding level is 16KB. If the current level has excess allocation space, 
	// then find a free block in the current level and return it and delete it from the free block list.
	uint32_t DX12BuddyAllocator::AllocateBlock(uint32_t Order)
	{
		uint32_t Offset;

		if (Order > MaxOrder)
		{
			assert(false);
		}

		if (FreeBlocks[Order].size() == 0)
		{
			// No free nodes in the requested pool.  Try to find a higher-order block and split it.  
			uint32_t Left = AllocateBlock(Order + 1);

			uint32_t UnitSize = OrderToUnitSize(Order);

			uint32_t Right = Left + UnitSize;

			FreeBlocks[Order].insert(Right); // Add the right block to the free pool  

			Offset = Left; // Return the left block  
		}
		else
		{
			auto It = FreeBlocks[Order].cbegin();
			Offset = *It;

			// Remove the block from the free list
			FreeBlocks[Order].erase(*It);
		}

		return Offset;
	}

	uint32_t DX12BuddyAllocator::GetSizeToAllocate(uint32_t Size, uint32_t Alignment)
	{
		uint32_t SizeToAllocate = Size;

		// If the alignment doesn't match the block size
		if (Alignment != 0 && MinBlockSize % Alignment != 0)
		{
			SizeToAllocate = Size + Alignment;
		}

		return SizeToAllocate;
	}

	bool DX12BuddyAllocator::CanAllocate(uint32_t SizeToAllocate)
	{
		if (TotalAllocSize == DEFAULT_POOL_SIZE)
		{
			return false;
		}

		uint32_t BlockSize = DEFAULT_POOL_SIZE;

		for (int i = (int)FreeBlocks.size() - 1; i >= 0; i--)
		{
			if (FreeBlocks[i].size() && BlockSize >= SizeToAllocate)
			{
				return true;
			}

			// Halve the block size;
			BlockSize = BlockSize >> 1;

			if (BlockSize < SizeToAllocate) return false;
		}
		return false;
	}

	void DX12BuddyAllocator::DeallocateInternal(const DX12BuddyBlockData& Block)
	{
		DeallocateBlock(Block.Offset, Block.Order);

		uint32_t Size = OrderToUnitSize(Block.Order) * MinBlockSize;
		TotalAllocSize -= Size;

		if (InitData.AllocationStrategy == EAllocationStrategy::kPlacedResource)
		{
			// Release place resource
			assert(Block.PlacedResource != nullptr);

			delete Block.PlacedResource;
		}
	}

	// When recycling each block, it will check if the buddy is free. 
	// If it is, it will be merged with the buddy into a large free block. If not,
	// the current block will be directly added to the free block list.
	void DX12BuddyAllocator::DeallocateBlock(uint32_t Offset, uint32_t Order)
	{
		// Get buddy block
		uint32_t Size = OrderToUnitSize(Order);
		uint32_t Buddy = GetBuddyOffset(Offset, Size);

		auto It = FreeBlocks[Order].find(Buddy);
		if (It != FreeBlocks[Order].end()) // If buddy block is free, merge it
		{
			// Deallocate merged blocks
			DeallocateBlock(std::min(Offset, Buddy), Order + 1);

			// Remove the buddy from the free list  
			FreeBlocks[Order].erase(*It);
		}
		else
		{
			// Add the block to the free list
			FreeBlocks[Order].insert(Offset);
		}
	}

	// ------------------- DX12MultiBuddyAllocator ----------------------------------------------

	DX12MultiBuddyAllocator::DX12MultiBuddyAllocator(ID3D12Device* Device, const DX12BuddyAllocator::AllocatorInitData& InitData)
		: m_Device(Device), m_InitData(InitData)
	{

	}

	DX12MultiBuddyAllocator::~DX12MultiBuddyAllocator()
	{

	}

	bool DX12MultiBuddyAllocator::AllocResource(uint32_t Size, uint32_t Alignment, DX12ResourceAllocation& ResourceLocation)
	{
		for (auto& Allocator : m_Allocators)
		{
			if (Allocator->AllocResource(Size, Alignment, ResourceLocation))
			{
				return true;
			}
		}

		// Create new allocator
		auto Allocator = std::make_shared<DX12BuddyAllocator>(m_Device, m_InitData);
		m_Allocators.push_back(Allocator);

		bool Result = Allocator->AllocResource(Size, Alignment, ResourceLocation);
		assert(Result);

		return true;
	}

	void DX12MultiBuddyAllocator::CleanUpAllocations()
	{
		for (auto& Allocator : m_Allocators)
		{
			Allocator->CleanUpAllocations();
		}
	}

	// ------------------- DX12UploadBufferAllocator ----------------------------------
	DX12UploadBufferAllocator::DX12UploadBufferAllocator(ID3D12Device* InDevice)
	{
		DX12BuddyAllocator::AllocatorInitData InitData;
		InitData.AllocationStrategy = DX12BuddyAllocator::EAllocationStrategy::kManualSubAllocation;
		InitData.HeapType = D3D12_HEAP_TYPE_UPLOAD;
		InitData.ResourceFlags = D3D12_RESOURCE_FLAG_NONE;

		Allocator = std::make_unique<DX12MultiBuddyAllocator>(InDevice, InitData);

		D3DDevice = InDevice;
	}

	void* DX12UploadBufferAllocator::AllocUploadResource(uint32_t Size, uint32_t Alignment, DX12ResourceAllocation& ResourceLocation)
	{
		Allocator->AllocResource(Size, Alignment, ResourceLocation);

		return ResourceLocation.m_MappedAddress;
	}

	void DX12UploadBufferAllocator::CleanUpAllocations()
	{
		Allocator->CleanUpAllocations();
	}



	DX12DefaultBufferAllocator::DX12DefaultBufferAllocator(ID3D12Device* InDevice)
	{
		{
			DX12BuddyAllocator::AllocatorInitData InitData;
			InitData.AllocationStrategy = DX12BuddyAllocator::EAllocationStrategy::kManualSubAllocation;
			InitData.HeapType = D3D12_HEAP_TYPE_DEFAULT;
			InitData.ResourceFlags = D3D12_RESOURCE_FLAG_NONE;

			Allocator = std::make_unique<DX12MultiBuddyAllocator>(InDevice, InitData);
		}

		{
			DX12BuddyAllocator::AllocatorInitData InitData;
			InitData.AllocationStrategy = DX12BuddyAllocator::EAllocationStrategy::kManualSubAllocation;
			InitData.HeapType = D3D12_HEAP_TYPE_DEFAULT;
			InitData.ResourceFlags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

			UavAllocator = std::make_unique<DX12MultiBuddyAllocator>(InDevice, InitData);
		}

		D3DDevice = InDevice;
	}

	void DX12DefaultBufferAllocator::AllocDefaultResource(const D3D12_RESOURCE_DESC& ResourceDesc,
		uint32_t Alignment, DX12ResourceAllocation& ResourceLocation)
	{
		if (ResourceDesc.Flags == D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS)
		{
			UavAllocator->AllocResource((uint32_t)ResourceDesc.Width, Alignment, ResourceLocation);
		}
		else
		{
			Allocator->AllocResource((uint32_t)ResourceDesc.Width, Alignment, ResourceLocation);
		}
	}

	void DX12DefaultBufferAllocator::CleanUpAllocations()
	{
		Allocator->CleanUpAllocations();
	}



	DX12TextureResourceAllocator::DX12TextureResourceAllocator(ID3D12Device* InDevice)
	{
		DX12BuddyAllocator::AllocatorInitData InitData;
		InitData.AllocationStrategy = DX12BuddyAllocator::EAllocationStrategy::kPlacedResource;
		InitData.HeapType = D3D12_HEAP_TYPE_DEFAULT;
		InitData.HeapFlags = D3D12_HEAP_FLAG_ALLOW_ONLY_NON_RT_DS_TEXTURES;

		Allocator = std::make_unique<DX12MultiBuddyAllocator>(InDevice, InitData);

		D3DDevice = InDevice;
	}

	void DX12TextureResourceAllocator::AllocTextureResource(const D3D12_RESOURCE_STATES& ResourceState,
		const D3D12_RESOURCE_DESC& ResourceDesc, DX12ResourceAllocation& ResourceLocation)
	{
		const D3D12_RESOURCE_ALLOCATION_INFO Info = D3DDevice->GetResourceAllocationInfo(0, 1, &ResourceDesc);

		Allocator->AllocResource((uint32_t)Info.SizeInBytes, DEFAULT_RESOURCE_ALIGNMENT, ResourceLocation);

		// Create placed resource
		{
			Microsoft::WRL::ComPtr<ID3D12Resource> Resource;
			ID3D12Heap* BackingHeap = ResourceLocation.m_Allocator->GetBackingHeap();
			uint64_t HeapOffset = ResourceLocation.OffsetFromBaseOfHeap;
			D3DDevice->CreatePlacedResource(BackingHeap, HeapOffset, &ResourceDesc, ResourceState, nullptr, IID_PPV_ARGS(&Resource));

			DX12Resource* NewResource = new DX12Resource(Resource);
			ResourceLocation.m_UnderlyingResource = NewResource;
			ResourceLocation.m_BlockData.PlacedResource = NewResource;  // Will delete Resource when ResourceLocation was destroyed
		}
	}

	void DX12TextureResourceAllocator::CleanUpAllocations()
	{
		Allocator->CleanUpAllocations();
	}
}