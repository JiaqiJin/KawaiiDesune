#pragma once

#include "../Utils/RingAllocator.h"

using namespace Util;

namespace RHI
{
	class UploadBuffer
	{
	public:
		UploadBuffer(ID3D12Device* device, size_t max_size_in_bytes);

	private:
		RingAllocator m_ringAllocator;
		std::mutex m_mutex;

		Microsoft::WRL::ComPtr<ID3D12Resource> m_buffer = nullptr;
		BYTE* m_cpuAddress = nullptr;
		D3D12_GPU_VIRTUAL_ADDRESS m_gpuAddress = 0;
	};
}