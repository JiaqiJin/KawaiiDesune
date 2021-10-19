#pragma once

#include <d3d12.h>
#include <vector>

namespace RHI
{
	class ResourceBarriers
	{
	public:
		ResourceBarriers() = default;

		void AddTransition(ID3D12Resource* Resource,
			D3D12_RESOURCE_STATES StateBefore_,
			D3D12_RESOURCE_STATES StateAfter_,
			UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES)
		{
			D3D12_RESOURCE_BARRIER resourceBarrier;
			resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			resourceBarrier.Transition.pResource = Resource;
			resourceBarrier.Transition.StateBefore = StateBefore_;
			resourceBarrier.Transition.StateAfter = StateAfter_;
			resourceBarrier.Transition.Subresource = subresource;

			m_resourceBarriers.push_back(resourceBarrier);
		}

		void Submit(ID3D12GraphicsCommandList* cmdList) const
		{
			cmdList->ResourceBarrier(static_cast<UINT>(m_resourceBarriers.size()), m_resourceBarriers.data());
		}

		void Clear()
		{
			m_resourceBarriers.clear();
		}
	private:
		std::vector<D3D12_RESOURCE_BARRIER> m_resourceBarriers;
	};
}