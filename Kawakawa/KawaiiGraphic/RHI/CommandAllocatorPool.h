#pragma once

namespace RHI
{
	/*
	* Represent the allocation of storage for GPU commands
	* It dynamically creates more CommandAllocators when they run out, 
	* and releases them back into the pool of available allocators when they are done processing on the GPU
	*/
	class CommandAllocatorPool
	{
	public:
		CommandAllocatorPool(D3D12_COMMAND_LIST_TYPE type);
		~CommandAllocatorPool();

		void Create(ID3D12Device* pDevice);

		void ShutDown();
		// Check the front Allocator in the Pool, if the current GPU has completed the Allocator command,
		// it can be reused, otherwise create a new one.
		ID3D12CommandAllocator* RequestAllocator(uint64_t completedFenceValue);

		// Discards a given allocator. It will be available after a given fence point
		void DiscardAllocator(uint64_t fenceValue, ID3D12CommandAllocator* allocator);

		inline size_t Size() { return m_AllocatorPool.size(); }
	private:
		const D3D12_COMMAND_LIST_TYPE m_cCommandListType;

		ID3D12Device* m_Device;
		std::vector<ID3D12CommandAllocator*> m_AllocatorPool;
		//					Fence Value, Allocator
		std::queue<std::pair<uint64_t, ID3D12CommandAllocator*>> m_ReadyAllocators;
		std::mutex m_AllocatorMutex;
	};
}