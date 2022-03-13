#pragma once

#include "CommandQueue.h"

namespace RHI
{
	// Manage all the different CommandQueues: GraphicsQueue, ComputeQueue, and CopyQueue.
	class CommandListManager
	{
	public:
		CommandListManager();
		~CommandListManager();

		void Create(ID3D12Device* pDevice);
		void Shutdown();

		CommandQueue& GetGraphicsQueue() { return m_GraphicsQueue; }
		CommandQueue& GetComputeQueue() { return m_ComputeQueue; }
		CommandQueue& GetCopyQueue() { return m_CopyQueue; }

		CommandQueue& GetQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT)
		{
			switch (type)
			{
			case D3D12_COMMAND_LIST_TYPE_COMPUTE: return m_ComputeQueue;
			case D3D12_COMMAND_LIST_TYPE_COPY: return m_CopyQueue;
			default: return m_GraphicsQueue;
			}
		}

		ID3D12CommandQueue* GetCommandQueue() { return m_GraphicsQueue.GetCommandQueue(); }

		void CreateNewCommandList(
			D3D12_COMMAND_LIST_TYPE type,
			ID3D12GraphicsCommandList** list, 
			ID3D12CommandAllocator** allocator);

		bool IsFenceComplete(uint64_t fenceValue)
		{
			return GetQueue(D3D12_COMMAND_LIST_TYPE(fenceValue >> 56)).IsFenceComplete(fenceValue);
		}

		// the cpu will wait for a fence to reach a specified value
		void WaitForFence(uint64_t fenceValue);

		// the cpu will wait for all command queues to empty (so that the cpu is idle)
		void IdleGPU()
		{
			m_GraphicsQueue.WaitForIdle();
			m_ComputeQueue.WaitForIdle();
			m_CopyQueue.WaitForIdle();
		}
	private:
		ID3D12Device* m_Device;

		CommandQueue m_GraphicsQueue;
		CommandQueue m_ComputeQueue;
		CommandQueue m_CopyQueue;
	};
}