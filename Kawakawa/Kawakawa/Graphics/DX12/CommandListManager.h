﻿#pragma once
#include "CommandQueue.h"

#include "../../Utility/Singleton.h"

namespace Kawaii::Graphics::backend::DX12
{
	/**
	 * 单例类，持有三个CommandQueue：Graphic、Compute、Copy，对应GPU中的三种可以并行的管线
	*/
    class CommandListManager : public Singleton<CommandListManager>
    {
    public:
        CommandListManager();

		CommandListManager(const CommandListManager&) = delete;
		CommandListManager& operator=(const CommandListManager&) = delete;
		CommandListManager(CommandListManager&&) = default;
		CommandListManager& operator=(CommandListManager&&) = default;

		CommandQueue& GetGraphicsQueue(void) { return m_GraphicsQueue; }
		CommandQueue& GetComputeQueue(void) { return m_ComputeQueue; }
		CommandQueue& GetCopyQueue(void) { return m_CopyQueue; }

		CommandQueue& GetQueue(D3D12_COMMAND_LIST_TYPE Type = D3D12_COMMAND_LIST_TYPE_DIRECT)
		{
			switch (Type)
			{
			case D3D12_COMMAND_LIST_TYPE_COMPUTE: return m_ComputeQueue;
			case D3D12_COMMAND_LIST_TYPE_COPY: return m_CopyQueue;
			default: return m_GraphicsQueue;
			}
		}

		ID3D12CommandQueue* GetCommandQueue() { return m_GraphicsQueue.GetCommandQueue(); }

		void CreateNewCommandList(
			D3D12_COMMAND_LIST_TYPE Type,
			ID3D12GraphicsCommandList5** list,
			ID3D12CommandAllocator** Allocator);

		void Initialize(ID3D12Device6* device);

		// Test to see if a fence has already been reached
		bool IsFenceComplete(uint64_t fenceValue) 
		{
			return GetQueue(static_cast<D3D12_COMMAND_LIST_TYPE>(fenceValue >> 56)).IsFenceComplete(fenceValue);
		}

		// The CPU will wait for a fence to reach a specified value
		void WaitForFence(uint64_t FenceValue);

		// The CPU will wait for all command queues to empty (so that the GPU is idle)
		void IdleGPU(void)
		{
			m_GraphicsQueue.WaitIdle();
			m_ComputeQueue.WaitIdle();
			m_CopyQueue.WaitIdle();
		}

		ID3D12Device* GetDevice() const noexcept { return m_Device; }

    private:
		ID3D12Device* m_Device = nullptr;

		CommandQueue   m_GraphicsQueue;
		CommandQueue   m_ComputeQueue;
		CommandQueue   m_CopyQueue;
    };
}