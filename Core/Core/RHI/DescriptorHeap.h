#pragma once

/*
* Descriptor are small block of memory which tell shader where to find the resource, and how interprete data resource.
* Descriptor Heap are chunk of memory(array) where Descriptor are stored. It could be Shader Visible(CBV, UAV, SRV, Sampler)
* Shader No Visibel (RTV, DSV) 
*/
namespace RHI
{
	class DescriptorHeap
	{
	public:
		// Constructors
		DescriptorHeap(ID3D12DescriptorHeap* pExistingHeap);
		DescriptorHeap(ID3D12Device* device, size_t count);
		DescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_DESC const& _desc);
		DescriptorHeap(ID3D12Device* device,
			D3D12_DESCRIPTOR_HEAP_TYPE type,
			D3D12_DESCRIPTOR_HEAP_FLAGS flags,
			size_t count);

		DescriptorHeap(DescriptorHeap const&) = delete;
		DescriptorHeap(DescriptorHeap&&) = default;

		DescriptorHeap& operator=(DescriptorHeap const&) = delete;
		DescriptorHeap& operator=(DescriptorHeap&&) = default;

		~DescriptorHeap() = default;

		// Getters 
		D3D12_GPU_DESCRIPTOR_HANDLE GetFirstGpuHandle() const;
		D3D12_CPU_DESCRIPTOR_HANDLE GetFirstCpuHandle() const;
		D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(size_t index) const;
		D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(size_t index) const;

		size_t Count() const { return m_desc.NumDescriptors; }
		D3D12_DESCRIPTOR_HEAP_FLAGS Flags() const { return m_desc.Flags; }
		D3D12_DESCRIPTOR_HEAP_TYPE  Type() const { return m_desc.Type; }
		size_t Increment() const { return m_descriptorHandleSize; }
		ID3D12DescriptorHeap* Heap() const { return m_heap.Get(); }

	private:
		// Desctriptor Heap
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heap;
		// Descriptor Desc
		D3D12_DESCRIPTOR_HEAP_DESC  m_desc;
		// Descriptor CPU Handle
		D3D12_CPU_DESCRIPTOR_HANDLE m_CPUHandle;
		// Descriptor GPU Handle
		D3D12_GPU_DESCRIPTOR_HANDLE m_GPUHandle;
		// Increament Handle Size
		UINT m_descriptorHandleSize;

		void CreateDescriptorHelper(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_DESC const& _desc);
	};
}