#include "../Kawaiipch.h"
#include "UploadBuffer.h"

namespace RHI
{
	UploadBuffer::UploadBuffer(ID3D12Device* device, size_t max_size_in_bytes)
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

	// --------------------- Lineal Upload Buffer -------------------
	LinealUploadBuffer::LinealUploadBuffer(ID3D12Device* device, SIZE_T max_size_in_bytes)
		: UploadBuffer(device, max_size_in_bytes), m_linearAllocator(max_size_in_bytes)
	{

	}

	void LinealUploadBuffer::Clear()
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		m_linearAllocator.Clear();
	}

	D3D12_GPU_VIRTUAL_ADDRESS LinealUploadBuffer::GPUAddress() const
	{
		return m_gpuAddress;
	}

	// --------------------- Ring Upload Buffer -------------------
	RingUploadBuffer::RingUploadBuffer(ID3D12Device* device, SIZE_T max_size_in_bytes)
		: UploadBuffer(device, max_size_in_bytes), m_ringAllocator(max_size_in_bytes)
	{

	}

	void RingUploadBuffer::FinishCurrentFrame(uint64_t frame)
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		m_ringAllocator.FinishCurrentFrame(frame);
	}

	void RingUploadBuffer::ReleaseCompletedFrames(uint64_t completedFrame)
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		m_ringAllocator.ReleaseCompletedFrames(completedFrame);
	}

}