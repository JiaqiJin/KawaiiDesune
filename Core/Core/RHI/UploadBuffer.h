#pragma once

#include "../Utils/LinearAllocator.h"
#include "../Utils/RingAllocator.h"

using namespace Util;

namespace RHI
{
	class UploadBuffer
	{
	public:
		UploadBuffer(ID3D12Device* device, size_t max_size_in_bytes);

	protected:
		std::mutex m_mutex;

		Microsoft::WRL::ComPtr<ID3D12Resource> m_buffer = nullptr;
		BYTE* m_cpuAddress = nullptr;
		D3D12_GPU_VIRTUAL_ADDRESS m_gpuAddress = 0;
	};

	// Lineal Upload Buffer
	class LinealUploadBuffer : public UploadBuffer
	{
	public:
		LinealUploadBuffer(ID3D12Device* device, SIZE_T max_size_in_bytes);
		
		void Clear();

		D3D12_GPU_VIRTUAL_ADDRESS GPUAddress() const;
	private:
		LinearAllocator m_linearAllocator;
	};

	// Ring Upload Buffer
	class RingUploadBuffer : public UploadBuffer
	{
	public:
		RingUploadBuffer(ID3D12Device* device, SIZE_T max_size_in_bytes);

		void FinishCurrentFrame(uint64_t frame);

		void ReleaseCompletedFrames(uint64_t completedFrame);
	private:
		RingAllocator m_ringAllocator;
	};
}