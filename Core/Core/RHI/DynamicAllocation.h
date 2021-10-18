#pragma once

#include <d3d12.h>
#include <wrl.h>
#include "../Common/d3dx12.h"
#include "../Utils/AllocatorUtil.h"

namespace RHI
{
	struct DynamicAllocation
	{
		ID3D12Resource* pBuffer = nullptr;
		size_t Offset = 0;
		size_t Size = 0;
		void* CPUAddress = 0;
		D3D12_GPU_VIRTUAL_ADDRESS GPUAddress = 0;
	};
}