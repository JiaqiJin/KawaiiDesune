#pragma once

#include "GpuResource.h"
#include "DescriptorAllocator.h"
#include "EnumConverter.h"

namespace Kawaii::Graphics::backend::DX12
{
	class GpuBuffer : public GpuResource
	{
	public:
		GpuBuffer(ID3D12Device* device, std::string_view name, size_t num_elements, size_t element_size);
		size_t GetBufferSize() const { return m_BufferSize; }
		size_t GetElementCount() const { return m_NumElements; }
		size_t GetElementSize() const { return m_ElementSize; }

	protected:
		size_t m_NumElements;
		size_t m_ElementSize;
		size_t m_BufferSize;
		D3D12_RESOURCE_FLAGS m_ResouceFlags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	};

	class VertexBuffer : public GpuBuffer
	{
	public:
		using GpuBuffer::GpuBuffer;
		D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const
		{
			D3D12_VERTEX_BUFFER_VIEW vbv;
			vbv.BufferLocation = m_Resource->GetGPUVirtualAddress();
			vbv.SizeInBytes = m_BufferSize;
			vbv.StrideInBytes = m_ElementSize;
			return vbv;
		}
	};

	class IndexBuffer : public GpuBuffer
	{
	public:
		using GpuBuffer::GpuBuffer;
		D3D12_INDEX_BUFFER_VIEW IndexBufferView() const 
		{
			D3D12_INDEX_BUFFER_VIEW ibv;
			ibv.BufferLocation = m_Resource->GetGPUVirtualAddress();
			ibv.Format = index_size_to_dxgi_format(m_ElementSize);
			ibv.SizeInBytes = m_BufferSize;
			return ibv;
		}
	};

	class ConstantBuffer : public GpuResource
	{
	public:
	};

	class TextureBuffer : public GpuResource
	{
	public:
	};

	class DepthBuffer: public GpuResource
	{
	public:
		DepthBuffer(std::string_view name, ID3D12Device* device, DescriptorAllocation&& dsv, const D3D12_RESOURCE_DESC& desc, float clear_depth, uint8_t clear_stencil);
		const DescriptorAllocation& GetDSV() const noexcept { return m_DSV; }
	private:
		DescriptorAllocation m_DSV;
		float m_ClearDepth;
		uint8_t m_ClearStencil;
	};
}