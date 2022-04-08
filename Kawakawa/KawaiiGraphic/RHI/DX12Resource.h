#pragma once

/* ID3D12Resource Memory allocation scheme
* StandAlone -> CommittedResource to creates TD3D12Resource, give it to a rendering resource to use.
* SubAllocation :
*	PlacedResource -> Create an ID3D12Heap and use the PlacedResource interface on the Heap to create multiple ID3D12Resources
*	ManualSubAllocation -> ID3D12Resource is created through the CommittedResource interface, 
		logically divide a ID3D12Resource into multiple copies for use by multiple rendering resources.
*/

namespace RHI
{
	class RenderDevice;
	class DX12BuddyAllocator;

	// ID3D12Resource represent GPU resources (Textures, buffer)
	class DX12Resource
	{
	public:
		DX12Resource(Microsoft::WRL::ComPtr<ID3D12Resource> Resource, D3D12_RESOURCE_STATES InitState = D3D12_RESOURCE_STATE_COMMON);

		void Map();
	public:
		// DX12 Resource
		Microsoft::WRL::ComPtr<ID3D12Resource> m_Resource = nullptr;
		// GPU Virtual Address
		D3D12_GPU_VIRTUAL_ADDRESS m_GPUVirtualAddress = 0;
		// Current State
		D3D12_RESOURCE_STATES m_CurrentState;
		// Mapped Memory
		void* m_MappedAdderessMemory = nullptr;
	};

	struct DX12BuddyBlockData
	{
		uint32_t Offset = 0;
		uint32_t Order = 0;
		uint32_t CurrentUsedSize = 0;

		DX12Resource* PlacedResource = nullptr;
	};

	// Support SubAllocation by recording offset,
	// If it is SubAllocation (PlacedResource), then OffsetFromBaseOfHeap records its relative to Heap,
	// If it is SubAllocation (ManualSubAllocation)
	// then OffsetFromBaseOfResource records its offset relative to the starting position
	class DX12ResourceAllocation
	{
	public:
		enum class EResourceLocationType
		{
			Undefined,
			StandAlone,
			SubAllocation,
		};

		void SetType(EResourceLocationType Type) { m_ResourceLocationType = Type; }
	public:
		EResourceLocationType m_ResourceLocationType = EResourceLocationType::Undefined;

		// SubAllocation
		DX12BuddyAllocator* m_Allocator = nullptr;

		DX12BuddyBlockData m_BlockData;

		// StandAlone Resource
		DX12Resource* m_UnderlyingResource = nullptr;

		// if PlacedResource records its offset relative to the starting position of Heap,
		// f it is ManualSubAllocation, then OffsetFromBaseOfResource records its offset relative to the starting position of DX12Resource
		union
		{
			uint64_t OffsetFromBaseOfResource;
			uint64_t OffsetFromBaseOfHeap;
		};

		D3D12_GPU_VIRTUAL_ADDRESS m_GPUVirtualAddress = 0;

		// About mapping, for upload buffer
		void* m_MappedAddress = nullptr;
	};
}