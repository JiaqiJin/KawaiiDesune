#pragma once

namespace Kawaii::Graphics::backend::DX12
{
    class CommandAllocatorPool
    {
    public:
		CommandAllocatorPool(D3D12_COMMAND_LIST_TYPE Type, ID3D12Device* Device);

		ID3D12CommandAllocator* RequestAllocator(UINT64 CompletedFenceValue);
		void DiscardAllocator(uint64_t FenceValue, ID3D12CommandAllocator* Allocator);

    private:
		const D3D12_COMMAND_LIST_TYPE m_cCommandListType;

		ID3D12Device* m_Device;
		std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> m_Allocators;
		std::queue<std::pair<uint64_t, ID3D12CommandAllocator*>> m_ReadyAllocators;
    };
}