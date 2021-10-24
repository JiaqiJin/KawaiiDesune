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
	class VertexBuffer
	{
	public:
		template<typename vertex_t>
		VertexBuffer(GraphicCore* gfx, vertex_t* vertices, size_t vertexCount)
		{
			auto allocator = gfx->GetAllocator();
			auto cmdList = gfx->GetDefaultCommandList();

			D3D12_RESOURCE_DESC resourceDesc = {};
			resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			resourceDesc.Alignment = 0;
			resourceDesc.Width = vertex_count * sizeof(vertex_t);
			resourceDesc.Height = 1;
			resourceDesc.DepthOrArraySize = 1;
			resourceDesc.MipLevels = 1;
			resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
			resourceDesc.SampleDesc.Count = 1;
			resourceDesc.SampleDesc.Quality = 0;
			resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

			D3D12MA::ALLOCATION_DESC allocationDesc = {};
			allocationDesc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

			D3D12MA::Allocation* alloc;
			ThrowIfFailed(allocator->CreateResource(
				&allocationDesc,
				&resourceDesc,
				D3D12_RESOURCE_STATE_COPY_DEST,
				NULL,
				&alloc,
				IID_PPV_ARGS(&m_vertexBuffer)));

			m_allocation.reset(alloc);

			m_vertexBuffer->SetName(L"Vertex Buffer");

			D3D12MA::ALLOCATION_DESC vBufferUploadAllocDesc = {};
			vBufferUploadAllocDesc.HeapType = D3D12_HEAP_TYPE_UPLOAD;

			D3D12_RESOURCE_DESC vertexBufferUploadResourceDesc = {};
			vertexBufferUploadResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			vertexBufferUploadResourceDesc.Alignment = 0;
			vertexBufferUploadResourceDesc.Width = vertex_count * sizeof(vertex_t);
			vertexBufferUploadResourceDesc.Height = 1;
			vertexBufferUploadResourceDesc.DepthOrArraySize = 1;
			vertexBufferUploadResourceDesc.MipLevels = 1;
			vertexBufferUploadResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
			vertexBufferUploadResourceDesc.SampleDesc.Count = 1;
			vertexBufferUploadResourceDesc.SampleDesc.Quality = 0;
			vertexBufferUploadResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			vertexBufferUploadResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

			D3D12MA::Allocation* vBufferUploadHeapAllocation = nullptr;
			ThrowIfFailed(allocator->CreateResource(
				&vBufferUploadAllocDesc,
				&vertexBufferUploadResourceDesc, // resource description for a buffer
				D3D12_RESOURCE_STATE_GENERIC_READ, // GPU will read from this buffer and copy its contents to the default heap
				nullptr,
				&vBufferUploadHeapAllocation, __uuidof(nullptr), nullptr));

			// Store vertex buffer in upload heap
			D3D12_SUBRESOURCE_DATA vertexData = {};
			vertexData.pData = reinterpret_cast<BYTE const*>(vertices); // pointer to our vertex array
			vertexData.RowPitch = vertexCount * sizeof(vertex_t); // size of all our triangle vertex data
			vertexData.SlicePitch = vertexCount * sizeof(vertex_t); // also the size of our triangle vertex data

			UINT64 res = UpdateSubresources(cmdList, m_vertexBuffer.Get(), vBufferUploadHeapAllocation->GetResource(),
				0, 0, 1, &vertexData);

			D3D12_RESOURCE_BARRIER vbBarrier = {};
			vbBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			vbBarrier.Transition.pResource = m_vertexBuffer.Get();
			vbBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			vbBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
			vbBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			cmdList->ResourceBarrier(1, &vbBarrier);

			// Initialize vertex buffer view
			m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
			m_vertexBufferView.StrideInBytes = sizeof(vertex_t);
			m_vertexBufferView.SizeInBytes = static_cast<UINT>(vertexCount) * sizeof(vertex_t);

			gfx->AddToReleaseQueue(vBufferUploadHeapAllocation);
		}

		VertexBuffer(VertexBuffer const&) = delete;
		VertexBuffer(VertexBuffer&& o) noexcept = default;

		VertexBuffer& operator=(VertexBuffer const&) = delete;
		VertexBuffer& operator=(VertexBuffer && o) noexcept = default;

		~VertexBuffer() = default;

		void Bind(ID3D12GraphicsCommandList* cmdList, UINT slot = 0u) const
		{
			cmdList->IASetVertexBuffers(slot, 1, &m_vertexBufferView);
		}

		D3D12_VERTEX_BUFFER_VIEW View() const
		{
			return m_vertexBufferView;
		}

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
		ReleasablePtr<D3D12MA::Allocation> m_allocation;
		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	};
}