#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include <array>
#include <cassert>
#include "GraphicsCore.h"
#include "Releasable.h"
#include "../Common/d3dx12.h"
#include "../Utils/DxException.h"

namespace RHI
{
	class IndexBuffer
	{
	public:
		template<typename index_t>
		IndexBuffer(GraphicCore* gfx, index_t* indices, size_t indexCount)
			: m_indexCount(static_cast<UINT>(indexCount))
		{
			auto allocator = gfx->GetAllocator();
			auto commandList = gfx->GetDefaultCommandList();

			size_t ib_byteSize = sizeof(index_t) * indexCount;

			// create default heap to hold index buffer
			D3D12MA::ALLOCATION_DESC indexBufferAllocDesc = {};
			indexBufferAllocDesc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

			D3D12_RESOURCE_DESC indexBufferResourceDesc = {};
			indexBufferResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			indexBufferResourceDesc.Alignment = 0;
			indexBufferResourceDesc.Width = ib_byteSize;
			indexBufferResourceDesc.Height = 1;
			indexBufferResourceDesc.DepthOrArraySize = 1;
			indexBufferResourceDesc.MipLevels = 1;
			indexBufferResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
			indexBufferResourceDesc.SampleDesc.Count = 1;
			indexBufferResourceDesc.SampleDesc.Quality = 0;
			indexBufferResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			indexBufferResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

			D3D12MA::Allocation* alloc;
			ThrowIfFailed(allocator->CreateResource(
				&indexBufferAllocDesc,
				&indexBufferResourceDesc, // resource description for a buffer
				D3D12_RESOURCE_STATE_COPY_DEST, // start in the copy destination state
				nullptr, // optimized clear value must be null for this type of resource
				&alloc,
				IID_PPV_ARGS(&m_indexBuffer)));

			m_allocation.reset(alloc);
			m_indexBuffer->SetName(L"Index Buffer");

			// Create Upload Heap to index buffer
			D3D12MA::ALLOCATION_DESC iBufferUploadAllocDesc = {};
			iBufferUploadAllocDesc.HeapType = D3D12_HEAP_TYPE_UPLOAD;

			D3D12_RESOURCE_DESC indexBufferUploadResourceDesc = {};
			indexBufferUploadResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			indexBufferUploadResourceDesc.Alignment = 0;
			indexBufferUploadResourceDesc.Width = ib_byteSize;
			indexBufferUploadResourceDesc.Height = 1;
			indexBufferUploadResourceDesc.DepthOrArraySize = 1;
			indexBufferUploadResourceDesc.MipLevels = 1;
			indexBufferUploadResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
			indexBufferUploadResourceDesc.SampleDesc.Count = 1;
			indexBufferUploadResourceDesc.SampleDesc.Quality = 0;
			indexBufferUploadResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			indexBufferUploadResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

			D3D12MA::Allocation* iBufferUploadHeapAllocation = nullptr;
			ThrowIfFailed(allocator->CreateResource(
				&iBufferUploadAllocDesc,
				&indexBufferUploadResourceDesc, // resource description for a buffer
				D3D12_RESOURCE_STATE_GENERIC_READ, // GPU will read from this buffer and copy its contents to the default heap
				nullptr,
				&iBufferUploadHeapAllocation,
				__uuidof(nullptr), nullptr));

			// store index buffer in upload heap
			D3D12_SUBRESOURCE_DATA indexData = {};
			indexData.pData = indices;              // pointer to our index array
			indexData.RowPitch = ib_byteSize;      // size of all our index buffer
			indexData.SlicePitch = ib_byteSize;    // also the size of our index buffer

			// Creating command to copy data from upload heap to default heap
			UINT64 res = UpdateSubresources(commandList, m_indexBuffer.Get(), 
				iBufferUploadHeapAllocation->GetResource(), 0, 0, 1, &indexData);
			assert(res);

			D3D12_RESOURCE_BARRIER ibBarrier = {};
			ibBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			ibBarrier.Transition.pResource = m_indexBuffer.Get();
			ibBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			ibBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_INDEX_BUFFER;
			ibBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			commandList->ResourceBarrier(1, &ibBarrier);

			// Initialize the index buffer view.
			m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();

			if (std::is_same_v<std::decay_t<index_t>, uint16_t>) m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
			else if (std::is_same_v<std::decay_t<index_t>, uint32_t>) m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
			else m_indexBufferView.Format = DXGI_FORMAT_UNKNOWN;
			m_indexBufferView.SizeInBytes = static_cast<UINT>(ib_byteSize);

			gfx->AddToReleaseQueue(iBufferUploadHeapAllocation);
		}

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> m_indexBuffer;
		ReleasablePtr<D3D12MA::Allocation> m_allocation = nullptr;
		D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
		uint32_t m_indexCount;
	};
}