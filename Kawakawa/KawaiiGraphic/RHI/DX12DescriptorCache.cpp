#include "pch.h"
#include "DX12DescriptorCache.h"
#include "DX12Device.h"

namespace RHI
{
	DX12DescriptorCache::DX12DescriptorCache(RenderDevice* Device)
		: m_Device(Device)
	{
		CreateCacheCbvSrvUavDescriptorHeap();
		CreateCacheRtvDescriptorHeap();
	}

	DX12DescriptorCache::~DX12DescriptorCache()
	{

	}

	void DX12DescriptorCache::CreateCacheCbvSrvUavDescriptorHeap()
	{
		// Create the descriptor heap.
		D3D12_DESCRIPTOR_HEAP_DESC HeapDesc = {};
		HeapDesc.NumDescriptors = m_MaxCbvSrvUavDescripotrCount;
		HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		ThrowIfFailed(m_Device->GetD3DDevice()->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&m_CacheCbvSrvUavDescriptorHeap)));
		SetDebugName(m_CacheCbvSrvUavDescriptorHeap.Get(), L"DX12DescriptorCache CacheCbvSrvUavDescriptorHeap");

		m_CbvSrvUavDescriptorSize = m_Device->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	void DX12DescriptorCache::CreateCacheRtvDescriptorHeap()
	{
		// Create the descriptor heap.
		D3D12_DESCRIPTOR_HEAP_DESC HeapDesc = {};
		HeapDesc.NumDescriptors = m_MaxRtvDescriptorCount;
		HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		ThrowIfFailed(m_Device->GetD3DDevice()->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&m_CacheRtvDescriptorHeap)));
		SetDebugName(m_CacheRtvDescriptorHeap.Get(), L"DX12DescriptorCache CacheRtvDescriptorHeap");

		m_RtvDescriptorSize = m_Device->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	CD3DX12_GPU_DESCRIPTOR_HANDLE DX12DescriptorCache::AppendCbvSrvUavDescriptors(const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& SrcDescriptors)
	{
		uint32_t SlotsNeeded = (uint32_t)SrcDescriptors.size();
		assert(m_CbvSrvUavDescriptorOffset + SlotsNeeded < m_MaxCbvSrvUavDescripotrCount);

		// CpuDescriptorHandle
		auto CpuDescriptorHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_CacheCbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), 
			m_CbvSrvUavDescriptorOffset, m_CbvSrvUavDescriptorSize);

		m_Device->GetD3DDevice()->CopyDescriptors(1, &CpuDescriptorHandle,
			&SlotsNeeded, SlotsNeeded, SrcDescriptors.data(), nullptr, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		// GpuDescriptorHandle
		auto GpuDescriptorHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_CacheCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			m_CbvSrvUavDescriptorOffset, m_CbvSrvUavDescriptorSize);

		// Increase Descriptor Offset
		m_CbvSrvUavDescriptorOffset += SlotsNeeded;

		return GpuDescriptorHandle;
	}

	void DX12DescriptorCache::AppendRtvDescriptors(const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& RtvDescriptors,
		CD3DX12_GPU_DESCRIPTOR_HANDLE& OutGpuHandle, CD3DX12_CPU_DESCRIPTOR_HANDLE& OutCpuHandle)
	{
		// Append to heap
		uint32_t SlotsNeeded = (uint32_t)RtvDescriptors.size();
		assert(m_RtvDescriptorOffset + SlotsNeeded < m_MaxRtvDescriptorCount);

		// CpuDescriptorHandle
		auto CpuDescriptorHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_CacheRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), 
			m_RtvDescriptorOffset, m_RtvDescriptorSize);

		// GpuDescriptorHandle
		m_Device->GetD3DDevice()->CopyDescriptors(1, &CpuDescriptorHandle, &SlotsNeeded, SlotsNeeded, RtvDescriptors.data(), 
			nullptr, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		OutGpuHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_CacheRtvDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			m_RtvDescriptorOffset, m_RtvDescriptorSize);

		OutCpuHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_CacheRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), 
			m_RtvDescriptorOffset, m_RtvDescriptorSize);

		// Increase descriptor offset
		m_RtvDescriptorOffset += SlotsNeeded;
	}

	void DX12DescriptorCache::ResetCacheCbvSrvUavDescriptorHeap()
	{
		m_CbvSrvUavDescriptorOffset = 0;
	}

	void DX12DescriptorCache::ResetCacheRtvDescriptorHeap()
	{
		m_RtvDescriptorOffset = 0;
	}


	void DX12DescriptorCache::Reset()
	{
		ResetCacheCbvSrvUavDescriptorHeap();
		ResetCacheRtvDescriptorHeap();
	}
}