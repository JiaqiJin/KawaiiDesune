#include "Sampler.h"

namespace Kawaii::Graphics::backend::DX12
{
	Sampler::Sampler(ID3D12Device* device, Descriptor&& sampler, const D3D12_SAMPLER_DESC& desc)
		:m_Sampler(std::move(sampler))
	{
		device->CreateSampler(&desc, sampler.m_Handle);
	}
}

