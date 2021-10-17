#include "../Kawaiipch.h"
#include "UploadBuffer.h"

namespace RHI
{
	UploadBuffer::UploadBuffer(ID3D12Device* device, size_t max_size_in_bytes)
		: m_ringAllocator(max_size_in_bytes)
	{
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(max_size_in_bytes);

		ThrowIfFailed(device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&bufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_buffer)));

		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_buffer->Map(0, &readRange, reinterpret_cast<void**>(&m_cpuAddress)));
		m_gpuAddress = m_buffer->GetGPUVirtualAddress();
	}
}