#pragma once

namespace RHI
{
	/*
	* The object Pool for Command Allocator
	* Ready Allocator : Check the front Allocator Pool, if the curret GPU has complete the Allocator Command, it 
	* can be reused, otherwise create new one.
	* Recycling: Put Allocator and the current FenceValue into the pool together
	*/
	class CommandAllocatorPool
	{
	public:
		CommandAllocatorPool(D3D12_COMMAND_LIST_TYPE Type, ID3D12Device* Device);
		~CommandAllocatorPool();

		ID3D12CommandAllocator* RequestAllocator(uint64_t CompletedFenceValue);
		void DiscardAllocator(uint64_t FenceValue, ID3D12CommandAllocator* Allocator);

		inline size_t Size() { return m_Allocators.size(); }
	private:
		const D3D12_COMMAND_LIST_TYPE m_cCommandListType;

		ID3D12Device* m_Device;
		std::vector<ID3D12CommandAllocator*> m_Allocators;
		std::queue<std::pair<uint64_t, ID3D12CommandAllocator*>> m_ReadyAllocators;
		std::mutex m_AllocatorMutex;
	};
}