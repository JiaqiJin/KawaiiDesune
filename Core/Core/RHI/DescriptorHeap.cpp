#include "../Kawaiipch.h"
#include "DescriptorHeap.h"

namespace RHI
{
	DescriptorHeap::DescriptorHeap(ID3D12DescriptorHeap* pExistingHeap) 
		: m_heap(pExistingHeap)
	{
		m_CPUHandle = pExistingHeap->GetCPUDescriptorHandleForHeapStart();
		m_GPUHandle = pExistingHeap->GetGPUDescriptorHandleForHeapStart();
		m_desc = pExistingHeap->GetDesc();

		Microsoft::WRL::ComPtr<ID3D12Device> device;
		pExistingHeap->GetDevice(IID_PPV_ARGS(device.GetAddressOf()));

		m_descriptorHandleSize = device->GetDescriptorHandleIncrementSize(m_desc.Type);
	}

	DescriptorHeap::DescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_DESC const& _desc)
		: m_desc{},
		m_CPUHandle{},
		m_GPUHandle{},
		m_descriptorHandleSize(0)
	{
		CreateDescriptorHelper(device, _desc);
	}

	DescriptorHeap::DescriptorHeap(
		ID3D12Device* device,
		D3D12_DESCRIPTOR_HEAP_TYPE type,
		D3D12_DESCRIPTOR_HEAP_FLAGS flags,
		size_t count)
		: m_desc{},
		m_CPUHandle{},
		m_GPUHandle{},
		m_descriptorHandleSize(0)
	{
		assert(count <= UINT32_MAX && "Too many descriptors");
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.Flags = flags;
		desc.NumDescriptors = static_cast<UINT>(count);
		desc.Type = type;
		CreateDescriptorHelper(device, desc);
	}

	DescriptorHeap::DescriptorHeap(ID3D12Device* device, size_t count)
		: DescriptorHeap(device,
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, count) { }

	D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetFirstGpuHandle() const
	{
		assert(m_desc.Flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		assert(m_heap != nullptr);

		return m_GPUHandle;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetFirstCpuHandle() const
	{
		assert(m_heap != nullptr);
		return m_CPUHandle;
	}

	D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetGpuHandle(size_t index) const
	{
		assert(m_heap != nullptr);
		assert(index < m_desc.NumDescriptors);
		assert(m_desc.Flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};
		gpuHandle.ptr += m_GPUHandle.ptr + UINT64(index) * UINT64(m_descriptorHandleSize);

		return gpuHandle;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetCpuHandle(size_t index) const
	{
		assert(m_heap != nullptr);
		assert(index < m_desc.NumDescriptors);

		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle{};
		cpuHandle.ptr = static_cast<SIZE_T>(m_CPUHandle.ptr + UINT64(index) * UINT64(m_descriptorHandleSize));
		return cpuHandle;
	}

	void DescriptorHeap::CreateDescriptorHelper(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_DESC const& desc)
	{
		m_desc = desc;
		m_descriptorHandleSize = pDevice->GetDescriptorHandleIncrementSize(m_desc.Type);

		if (m_desc.NumDescriptors == 0)
		{
			m_heap.Reset();
			m_CPUHandle.ptr = 0;
			m_GPUHandle.ptr = 0;
		}
		else
		{
			ThrowIfFailed(pDevice->CreateDescriptorHeap(&m_desc, IID_PPV_ARGS(m_heap.ReleaseAndGetAddressOf())));

			m_heap->SetName(L"DescriptorHeap");

			m_CPUHandle = m_heap->GetCPUDescriptorHandleForHeapStart();

			if (m_desc.Flags && D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)
				m_GPUHandle = m_heap->GetGPUDescriptorHandleForHeapStart();
		}
	}
}