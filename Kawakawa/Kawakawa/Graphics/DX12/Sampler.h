#pragma once

#include "../Resource.h"
#include "DescriptorAllocator.h"

namespace Kawaii::Graphics::backend::DX12
{
	class Sampler : public backend::Resource
	{
	public:
		Sampler(ID3D12Device* device, DescriptorAllocation&& sampler, const D3D12_SAMPLER_DESC& desc);
		const DescriptorAllocation& GetDescriptor() const noexcept { return m_Sampler; }

	private:
		DescriptorAllocation m_Sampler;
	};
}