#pragma once

namespace Kawaii::Graphics::backend::DX12
{
    /*
    *    CommandAllocator的对象池，
    *    请求：检查Pool中最前面的Allocator，如果当前GPU完成了这个Allocator的命令，就可以复用，否则重新创建一个
    *    回收：将Allocator和当前的FenceValue一起放入池子中
    *    创建CommandContext时会创建CommandList，这时会请求分配一个Allocator；当Reset CommandContext的CommandList时也会请求分配一个Allocator。
    *    在CommandContext Finish时会Discard Allocator，也就是命令记录完成了。
    */
    class CommandAllocatorPool
    {
    public:
        CommandAllocatorPool(D3D12_COMMAND_LIST_TYPE type);

        void Initialize(ID3D12Device* device) 
        {
            assert(device);
            m_Device = device;
        }

        ID3D12CommandAllocator* GetAllocator(uint64_t completedFenceValue);
        void                    DiscardAllocator(uint64_t fenceValue, ID3D12CommandAllocator* allocator);
        inline bool             Size() const noexcept { return m_AllocatorPool.size(); }

    private:
        ID3D12Device* m_Device = nullptr;
        D3D12_COMMAND_LIST_TYPE                                     m_Type;
        std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> m_AllocatorPool;
        std::queue<std::pair<uint64_t, ID3D12CommandAllocator*>>    m_ReadyAllocators;
    };
}