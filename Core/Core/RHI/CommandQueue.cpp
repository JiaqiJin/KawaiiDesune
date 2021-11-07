#include "../Kawaiipch.h"
#include "CommandQueue.h"

namespace RHI
{
    CommandQueue::CommandQueue(D3D12_COMMAND_LIST_TYPE Type, ID3D12Device* Device) :
        m_Type(Type),
        m_CommandQueue(nullptr),
        m_pFence(nullptr),
        m_NextFenceValue((uint64_t)Type << 56 | 1),
        m_LastCompletedFenceValue((uint64_t)Type << 56),
        m_AllocatorPool(Type, Device)
    {
        // Create Command Queue
        D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
        QueueDesc.Type = Type;
        QueueDesc.NodeMask = 1;
        ThrowIfFailed(Device->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&m_CommandQueue)));
        m_CommandQueue->SetName(L"CommandListManager::m_CommandQueue");

        // Create Fence
        ThrowIfFailed(Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence)));
        m_pFence->SetName(L"CommandListManager::m_pFence");

        m_FenceEventHandle = CreateEvent(nullptr, false, false, nullptr);
        assert(m_FenceEventHandle != NULL);
    }

    CommandQueue::~CommandQueue()
    {
        if (m_CommandQueue == nullptr)
            return;

        CloseHandle(m_FenceEventHandle);

        m_pFence->Release();
        m_pFence = nullptr;

        m_CommandQueue->Release();
        m_CommandQueue = nullptr;
    }

    uint64_t CommandQueue::IncrementFence(void)
    {
        std::lock_guard<std::mutex> LockGuard(m_FenceMutex);
        m_CommandQueue->Signal(m_pFence, m_NextFenceValue);
        return m_NextFenceValue++;
    }

    bool CommandQueue::IsFenceComplete(uint64_t FenceValue)
    {
        if (FenceValue > m_LastCompletedFenceValue)
            m_LastCompletedFenceValue = std::max(m_LastCompletedFenceValue, m_pFence->GetCompletedValue());

        return FenceValue <= m_LastCompletedFenceValue;
    }

    void CommandQueue::StallForFence(uint64_t FenceValue)
    {
        // TODO
    }

    void CommandQueue::StallForProducer(CommandQueue& Producer)
    {
        // TODO
    }

    void CommandQueue::WaitForFence(uint64_t FenceValue)
    {
        if (IsFenceComplete(FenceValue))
            return;

        std::lock_guard<std::mutex> LockGuard(m_EventMutex);

        m_pFence->SetEventOnCompletion(FenceValue, m_FenceEventHandle);
        WaitForSingleObject(m_FenceEventHandle, INFINITE);
        m_LastCompletedFenceValue = FenceValue;
    }

    uint64_t CommandQueue::ExecuteCommandList(ID3D12CommandList* List)
    {
        std::lock_guard<std::mutex> LockGuard(m_FenceMutex);

        ThrowIfFailed(((ID3D12GraphicsCommandList*)List)->Close());

        // Kickoff the command list
        m_CommandQueue->ExecuteCommandLists(1, &List);

        // Signal the next fence value (with the GPU)
        m_CommandQueue->Signal(m_pFence, m_NextFenceValue);

        // And increment the fence value.  
        return m_NextFenceValue++;
    }

    ID3D12CommandAllocator* CommandQueue::RequestAllocator()
    {
        uint64_t CompletedFence = m_pFence->GetCompletedValue();

        return m_AllocatorPool.RequestAllocator(CompletedFence);
    }

    void CommandQueue::DiscardAllocator(uint64_t FenceValue, ID3D12CommandAllocator* Allocator)
    {
        m_AllocatorPool.DiscardAllocator(FenceValue, Allocator);
    }
}