#include "D3Dpch.h"
#include "CommandAllocatorPool.h"

namespace Kawaii::Graphics::backend::DX12
{
	CommandAllocatorPool::CommandAllocatorPool(D3D12_COMMAND_LIST_TYPE type)
		: m_Type(type)
	{

	}

    // 请求一个CommandAllocator，检查对象池中的第一个Allocator是否可用，
    // 如果CompletedFenceValue > Pool中的FenceValue，表明GPU已经完成了这个Allocator的命令，表示可用
    // 否则重新创建一个CommandAllocator
    ID3D12CommandAllocator* CommandAllocatorPool::GetAllocator(uint64_t completedFenceValue) 
    {
        if (!m_ReadyAllocators.empty()) 
        {
            auto& [fenceValue, allocator] = m_ReadyAllocators.front();
            if (fenceValue <= completedFenceValue)
            {
                ThrowIfFailed(allocator->Reset());
                m_ReadyAllocators.pop();
                return allocator;
            }
        }

        // If no allocator(s) were ready to be used, create a new one
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> newAllocator;
        if (newAllocator == nullptr) 
        {
            ThrowIfFailed(m_Device->CreateCommandAllocator(m_Type, IID_PPV_ARGS(&newAllocator)));
            m_AllocatorPool.push_back(newAllocator);
        }
        return newAllocator.Get();
    }

    void CommandAllocatorPool::DiscardAllocator(uint64_t fenceValue, ID3D12CommandAllocator* allocator)
    {
        m_ReadyAllocators.push({ fenceValue, allocator });
    }
}