#pragma once

namespace RHI
{
	class CommandAllocatorPool;

	class CommandQueue
	{
	public:
		CommandQueue(D3D12_COMMAND_LIST_TYPE type);
		~CommandQueue();

		void Create(ID3D12Device* pDevice);
		void ShutDown();
		inline bool IsReady() { return m_CommandQueue != nullptr;}
		ID3D12CommandQueue* GetCommandQueue() { return m_CommandQueue; }
		uint64_t GetNextFenceValue() { return m_NextFenceValue; }
	private:
		// Command Queue
		ID3D12CommandQueue* m_CommandQueue;
		// Command List type
		const D3D12_COMMAND_LIST_TYPE m_Type;
		// Command Allocator Pool
		CommandAllocatorPool m_AllocatorPool;

		// Lifetime of these objects is managed by the descriptor cache
		ID3D12Fence* m_pFence;
		uint64_t m_NextFenceValue;
		uint64_t m_LastCompletedFenceValue;
		HANDLE m_FenceEventHandle;

		// Mutex
		std::mutex m_FenceMutex;
		std::mutex m_EventMutex;
	};
}