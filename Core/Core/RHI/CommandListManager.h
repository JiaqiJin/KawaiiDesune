﻿#pragma once

#include "CommandQueue.h"

namespace RHI
{
	/**
	* 单例类，持有三个CommandQueue：Graphic、Compute、Copy，对应GPU中的三种可以并行的管线
	*/
	class CommandListManager : public Singleton<CommandListManager>
	{
	public:
		CommandListManager(ID3D12Device* Device);
		~CommandListManager();

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

		ID3D12CommandQueue* GetCommandQueue()
		{
			return m_GraphicsQueue.GetCommandQueue();
		}

		void CreateNewCommandList(D3D12_COMMAND_LIST_TYPE Type, ID3D12GraphicsCommandList** List, ID3D12CommandAllocator** Allocator);
	private:
		ID3D12Device* m_Device;

		CommandQueue m_GraphicsQueue;
		CommandQueue m_ComputeQueue;
		CommandQueue m_CopyQueue;
	};
}