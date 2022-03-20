#pragma once

namespace RHI
{
	class RenderDevice;
	class DX12BuddyAllocator;

	class DX12Resource
	{
	public:
		DX12Resource(Microsoft::WRL::ComPtr<ID3D12Resource> Resource, D3D12_RESOURCE_STATES InitState = D3D12_RESOURCE_STATE_COMMON);

		void Map();
	public:
		Microsoft::WRL::ComPtr<ID3D12Resource> m_Resource = nullptr;
		D3D12_GPU_VIRTUAL_ADDRESS m_GPUVirtualAddress = 0;
		D3D12_RESOURCE_STATES m_CurrentState;
		void* m_MappedAdderessMemory = nullptr;
	};

	struct DX12BuddyBlockData
	{
		uint32_t Offset = 0;
		uint32_t Order = 0;
		uint32_t CurrentUsedSize = 0;

		DX12Resource* PlacedResource = nullptr;
	};

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