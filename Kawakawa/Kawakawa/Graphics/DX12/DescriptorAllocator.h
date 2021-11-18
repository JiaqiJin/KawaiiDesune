#pragma once

#include <optional>

// the DescriptorAllocator is CPU visiable only, for those used shader visiable descriptor,
// use the dynamic descriptor heap (in context).

namespace Kawaii::Graphics::backend::DX12
{
	class DescriptorPage;

	struct Descriptor
	{

	};

	class DescriptorPage
	{
	public:

	private:

	};

	class DescriptorAllocator
	{
	public:

	private:
		ID3D12Device* m_Device{};
		D3D12_DESCRIPTOR_HEAP_TYPE                 m_Type;
	};
}