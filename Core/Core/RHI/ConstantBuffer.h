#pragma once

#include <memory>

#include "GraphicsCore.h"
#include "../Common/d3dx12.h"
#include "../Utils/DxException.h"

namespace RHI
{
	template<typename BufferType>
	class ConstantBuffer
	{
		static constexpr uint32_t GetCBufferSize() { return (sizeof(BufferType) + 
			(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1)) & ~(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1);}
	public:
		ConstantBuffer(ID3D12Device* device, uint32_t bufferCount)
			: m_bufferSize(GetCBufferSize()), m_bufferCount(bufferCount) 
		{
			auto const& heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			auto const& bufferDesc = CD3DX12_RESOURCE_DESC::Buffer((uint64_t)m_bufferSize * m_bufferCount);

			ThrowIfFailed(device->CreateCommittedResource(
				&heapProp, // this heap will be used to upload the constant buffer data
				D3D12_HEAP_FLAG_NONE, // no flags
				&bufferDesc, // size of the resource heap. Must be a multiple of 64KB for single-textures and constant buffers
				D3D12_RESOURCE_STATE_GENERIC_READ, // will be data that is read from so we keep it in the generic read state
				nullptr, // we do not have use an optimized clear value for constant buffers
				IID_PPV_ARGS(&m_cb)));

			CD3DX12_RANGE read_range(0, 0);
			ThrowIfFailed(m_cb->Map(0, &read_range, reinterpret_cast<void**>(&m_mappedData)));
		}

		ConstantBuffer(ConstantBuffer const&) = delete;
		ConstantBuffer(ConstantBuffer&& o) noexcept;

		ConstantBuffer& operator=(ConstantBuffer const&) = delete;
		ConstantBuffer& operator=(ConstantBuffer&&) = delete;

		~ConstantBuffer();
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> m_cb;
		uint8_t* m_mappedData;
		uint32_t const m_bufferSize;
		uint32_t const m_bufferCount;
	};

	template<typename BufferType>
	ConstantBuffer<BufferType>::ConstantBuffer(ConstantBuffer&& o) noexcept
		: m_cb(std::move(o.m_cb)), m_bufferSize(o.m_bufferSize), m_mappedData(o.m_mappedData)
	{
		o.m_mappedData = nullptr;
	}

	template<typename BufferType>
	ConstantBuffer<BufferType>::~ConstantBuffer()
	{
		if (m_cb != nullptr)
			m_cb->Unmap(0, nullptr);

		m_mappedData = nullptr;
	}
}