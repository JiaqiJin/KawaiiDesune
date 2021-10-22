#pragma once

#include "GraphicsCore.h"
#include "../Common/d3dx12.h"
#include "../Utils/DxException.h"
#include <memory>
#include <cassert>

// Read back data from the GPU (for example, to capture a screen shot), you use a readback heap.
namespace RHI
{
	class ReadbackBuffer
	{
	public:
		ReadbackBuffer(ID3D12Device* device, UINT64 size) : m_size(size)
		{
			assert(size > 0);

			D3D12_RESOURCE_DESC resDesc = {};
			resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			resDesc.Width = UINT(size);
			resDesc.Height = 1;
			resDesc.DepthOrArraySize = 1;
			resDesc.MipLevels = 1;
			resDesc.Format = DXGI_FORMAT_UNKNOWN;
			resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
			resDesc.SampleDesc.Count = 1;
			resDesc.SampleDesc.Quality = 0;
			resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			resDesc.Alignment = 0;

			auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK);

			ThrowIfFailed(device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc,
				D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_resouce)));
		}

		void const* Map(void) const
		{
			assert(m_resouce != nullptr);
			void* data = nullptr;
			m_resouce->Map(0, nullptr, &data);
			return data;
		}

		template<typename T>
		T const* Map() const { return reinterpret_cast<const T*>(Map()); };

		void Unmap(void) const
		{
			assert(m_resouce != nullptr);
			m_resouce->Unmap(0, nullptr);
		}
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> m_resouce = nullptr;
		UINT64 m_size;
	};
}