#include "D3Dpch.h"
#include "CommandListManager.h"

namespace Kawaii::Graphics::backend::DX12
{
    CommandListManager::CommandListManager()
        : m_GraphicsQueue(D3D12_COMMAND_LIST_TYPE_DIRECT),
        m_ComputeQueue(D3D12_COMMAND_LIST_TYPE_COMPUTE),
        m_CopyQueue(D3D12_COMMAND_LIST_TYPE_COPY) 
    {

    }

    void CommandListManager::Initialize(ID3D12Device6* device) 
    {
        assert(device != nullptr);
        m_Device = device;

        m_GraphicsQueue.Initialize(device);
        m_ComputeQueue.Initialize(device);
        m_CopyQueue.Initialize(device);
    }

    void CommandListManager::CreateNewCommandList(D3D12_COMMAND_LIST_TYPE type, ID3D12GraphicsCommandList5** list,
        ID3D12CommandAllocator** allocator) 
    {
        switch (type) 
        {
        case D3D12_COMMAND_LIST_TYPE_DIRECT:
            *allocator = m_GraphicsQueue.RequestAllocator();
            break;
        case D3D12_COMMAND_LIST_TYPE_COMPUTE:
            *allocator = m_ComputeQueue.RequestAllocator();
            break;
        case D3D12_COMMAND_LIST_TYPE_COPY:
            *allocator = m_CopyQueue.RequestAllocator();
            break;
        default:
            throw "[D3D CommandListManager] Unsupport Command List Type!";
            break;
        }
        ThrowIfFailed(m_Device->CreateCommandList(0, type, *allocator, nullptr, IID_PPV_ARGS(list)));
        (*list)->SetName(L"CommandList");
    }

    void CommandListManager::WaitForFence(uint64_t fenceValue)
    {
        switch (static_cast<D3D12_COMMAND_LIST_TYPE>(fenceValue >> 56))
        {
        case D3D12_COMMAND_LIST_TYPE_DIRECT:
            m_GraphicsQueue.WaitForFence(fenceValue);
            break;
        case D3D12_COMMAND_LIST_TYPE_COMPUTE:
            m_ComputeQueue.WaitForFence(fenceValue);
            break;
        case D3D12_COMMAND_LIST_TYPE_COPY:
            m_CopyQueue.WaitForFence(fenceValue);
            break;
        default:
            throw "[D3D CommandListManager] Unsupport Command List Type!";
            break;
        }
    }
}