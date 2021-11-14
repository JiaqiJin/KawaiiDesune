#pragma once
#include "CommandAllocatorPool.h"

namespace Kawaii::Graphics::backend::DX12
{
    class CommandQueue
    {
		friend class CommandListManager;
		friend class CommandContext;
    public:
        CommandQueue(D3D12_COMMAND_LIST_TYPE type);
        ~CommandQueue();

        inline bool IsReady() const { return m_CommandQueue != nullptr; }

        void Initialize(ID3D12Device6* device);

        ID3D12CommandAllocator* RequestAllocator();
        void                    DiscardAllocator(uint64_t fenceValue, ID3D12CommandAllocator* allocator);

        uint64_t ExecuteCommandList(ID3D12CommandList* list);
        bool     IsFenceComplete(uint64_t fenceValue);
        uint64_t IncreaseFence();
        void     WaitForQueue(CommandQueue& other);
        void     WaitForFence(uint64_t fenceValue);
        void     WaitIdle() { WaitForFence(IncreaseFence()); }

        ID3D12CommandQueue* GetCommandQueue() { return m_CommandQueue.Get(); }
        uint64_t            GetNextFenceValue() const { return m_NextFenceValue; }
        uint64_t            GetLastCompletedFenceValue() const { return m_LastCompletedFenceValue; }

    private:
        D3D12_COMMAND_LIST_TYPE                    m_type;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
        CommandAllocatorPool                       m_AllocatorPool;

        // Fence
        Microsoft::WRL::ComPtr<ID3D12Fence1> m_Fence;
        uint64_t                             m_LastCompletedFenceValue;
        uint64_t                             m_NextFenceValue;
        HANDLE                               m_FenceHandle;
    };

}