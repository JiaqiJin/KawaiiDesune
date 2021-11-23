#pragma once

#include "D3Dpch.h"
#include "../Resource.h"

namespace Kawaii::Graphics::backend::DX12
{
	class GpuResource : public backend::Resource
	{
		friend class CommandContext;
	public:
		GpuResource(ID3D12Resource* resouce = nullptr)
			:m_TransitioningState(static_cast<D3D12_RESOURCE_STATES>(-1))
		{
			if (m_Resource)
				m_Resource.Attach(resouce);
		}

		ID3D12Resource* operator->() { return m_Resource.Get(); }
		const ID3D12Resource* operator->() const { return m_Resource.Get(); }

		ID3D12Resource* GetResource() { return m_Resource.Get(); }
		const ID3D12Resource* GetResource() const { return m_Resource.Get(); }
	protected :
		// Resouce
		ComPtr<ID3D12Resource> m_Resource;
		D3D12_RESOURCE_STATES  m_UsageState{ D3D12_RESOURCE_STATE_COMMON };
		D3D12_RESOURCE_STATES  m_TransitioningState;
	};
}