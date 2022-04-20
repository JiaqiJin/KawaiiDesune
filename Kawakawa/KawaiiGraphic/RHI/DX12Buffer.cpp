#include "pch.h"
#include "DX12Buffer.h"
#include "DX12GraphicRHI.h"
#include "DX12Device.h"

namespace RHI
{
	DX12ConstantBufferRef DX12GraphicRHI::CreateConstantBuffer(const void* Data, uint32_t Size)
	{
		DX12ConstantBufferRef ConstantBufferRef = std::make_shared<DX12Buffer>();

		//auto UploadBufferAllocator = GetDevice()->

		return ConstantBufferRef;
	}

	DX12VertexBufferRef DX12GraphicRHI::CreateVertexBuffer(const void* Data, uint32_t Size)
	{
		DX12VertexBufferRef VertexBufferRef = std::make_shared<DX12Buffer>();

		//auto UploadBufferAllocator = GetDevice()->

		return VertexBufferRef;
	}

	DX12IndexBufferRef DX12GraphicRHI::CreateIndexBuffer(const void* Data, uint32_t Size)
	{
		DX12IndexBufferRef IndexBufferRef = std::make_shared<DX12Buffer>();

		//auto UploadBufferAllocator = GetDevice()->

		return IndexBufferRef;
	}

	DX12ReadBackBufferRef DX12GraphicRHI::CreateReadBackBuffer(uint32_t Size)
	{
		DX12ReadBackBufferRef ReadBackBufferRef = std::make_shared<DX12Buffer>();

		//auto UploadBufferAllocator = GetDevice()->

		return ReadBackBufferRef;
	}
}
