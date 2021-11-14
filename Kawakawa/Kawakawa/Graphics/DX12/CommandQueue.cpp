#include "D3Dpch.h"
#include "CommandQueue.h"
#include "CommandListManager.h"

using Microsoft::WRL::ComPtr;

namespace Kawaii::Graphics::backend::DX12
{
    CommandQueue::CommandQueue(D3D12_COMMAND_LIST_TYPE type)
        : m_type(type),
        m_CommandQueue(nullptr),
        m_AllocatorPool(type),
        m_Fence(nullptr),
        // Use the upper 8 bit to identicate the type, which ranges from 0 to 6.
        m_LastCompletedFenceValue(static_cast<uint64_t>(type) << 56),
        m_NextFenceValue(static_cast<uint64_t>(type) << 56 | 1) {}

    CommandQueue::~CommandQueue() 
    { 
        CloseHandle(m_FenceHandle); 
    }

    void CommandQueue::Initialize(ID3D12Device6* device) 
    {
        assert(device != nullptr);
        assert(!IsReady());
        assert(m_AllocatorPool.Size() == 0);
        m_AllocatorPool.Initialize(device);

        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.Type = m_type;
        ThrowIfFailed(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_CommandQueue)));

        ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));
        m_Fence->Signal(static_cast<uint64_t>(m_type) << 56);

        m_FenceHandle = CreateEvent(nullptr, false, false, nullptr);
        if (m_FenceHandle == nullptr) ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));

        m_AllocatorPool.Initialize(device);

        assert(IsReady());
    }

    ID3D12CommandAllocator* CommandQueue::RequestAllocator()
    {
        uint64_t completedFenceValue = m_Fence->GetCompletedValue();
        return m_AllocatorPool.GetAllocator(completedFenceValue);
    }

    void CommandQueue::DiscardAllocator(uint64_t fenceValue, ID3D12CommandAllocator* allocator) 
    {
        m_AllocatorPool.DiscardAllocator(fenceValue, allocator);
    }

    uint64_t CommandQueue::ExecuteCommandList(ID3D12CommandList* list)
    {
        ThrowIfFailed(reinterpret_cast<ID3D12GraphicsCommandList5*>(list)->Close());
        m_CommandQueue->ExecuteCommandLists(1, &list);
        m_CommandQueue->Signal(m_Fence.Get(), m_NextFenceValue);
        return m_NextFenceValue++;
    }

    bool CommandQueue::IsFenceComplete(uint64_t fenceValue)
    {
        if (fenceValue > m_LastCompletedFenceValue)
            m_LastCompletedFenceValue = std::max(m_LastCompletedFenceValue, m_Fence->GetCompletedValue());
        return fenceValue <= m_LastCompletedFenceValue;
    }

    uint64_t CommandQueue::IncreaseFence() 
    {
        m_CommandQueue->Signal(m_Fence.Get(), m_NextFenceValue);
        return m_NextFenceValue++;
    }

    void CommandQueue::WaitForQueue(CommandQueue& other) 
    {
        assert(other.m_NextFenceValue > 0);

        m_CommandQueue->Wait(other.m_Fence.Get(), other.m_NextFenceValue - 1);
    }

    void CommandQueue::WaitForFence(uint64_t fenceValue)
    {
        if (IsFenceComplete(fenceValue)) return;
        m_Fence->SetEventOnCompletion(fenceValue, m_FenceHandle);
        WaitForSingleObject(m_FenceHandle, INFINITE);
        m_LastCompletedFenceValue = fenceValue;
    }
}

