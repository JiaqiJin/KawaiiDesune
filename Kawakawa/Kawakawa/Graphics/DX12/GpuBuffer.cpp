#include "GpuBuffer.h"

namespace Kawaii::Graphics::backend::DX12
{
	GpuBuffer::GpuBuffer(ID3D12Device* device, std::string_view name, size_t num_elements, size_t element_size)
		: m_NumElements(num_elements),
		m_ElementSize(element_size),
		m_BufferSize(num_elements* element_size)
	{
		auto desc = CD3DX12_RESOURCE_DESC::Buffer(m_BufferSize, m_ResouceFlags);
		auto heap_props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		ThrowIfFailed(device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &desc, m_UsageState, nullptr,
			IID_PPV_ARGS(&m_Resource)));

		m_Resource->SetName(std::wstring(name.begin(), name.end()).data());
	}

	// ---------------------------- Depth -------------------------------------

	DepthBuffer::DepthBuffer(std::string_view name, ID3D12Device* device, DescriptorAllocation&& dsv,
		const D3D12_RESOURCE_DESC& desc, float clear_depth, uint8_t clear_stencil)
		: m_DSV(std::move(dsv)), m_ClearDepth(clear_depth), m_ClearStencil(clear_stencil)
	{
		auto heap_props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		D3D12_CLEAR_VALUE clear_value = {};
		clear_value.Format = desc.Format;
		clear_value.DepthStencil.Depth = m_ClearDepth;
		clear_value.DepthStencil.Stencil = m_ClearStencil;
		ThrowIfFailed(device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, 
			&desc, m_UsageState, &clear_value, IID_PPV_ARGS(&m_Resource)));
		m_Resource->SetName(std::wstring(name.begin(), name.end()).data());

		D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
		dsv_desc.Format = desc.Format;
		dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsv_desc.Texture2D.MipSlice = 0;
		dsv_desc.Flags = D3D12_DSV_FLAG_NONE;

		device->CreateDepthStencilView(m_Resource.Get(), &dsv_desc, m_DSV.m_Handle);
	}
	
}