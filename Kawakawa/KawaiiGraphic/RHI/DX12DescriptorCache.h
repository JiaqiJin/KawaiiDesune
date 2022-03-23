#pragma once

namespace RHI
{
	class RenderDevice;

	class DX12DescriptorCache
	{
	public:
		DX12DescriptorCache(RenderDevice* Device);
		~DX12DescriptorCache();

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetCacheCbvSrvUavDescriptorHeap() { return m_CacheCbvSrvUavDescriptorHeap; }
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetCacheRtvDescriptorHeap() { return m_CacheRtvDescriptorHeap; }

		CD3DX12_GPU_DESCRIPTOR_HANDLE AppendCbvSrvUavDescriptors(const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& SrcDescriptors);
		void AppendRtvDescriptors(const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& RtvDescriptors, 
			CD3DX12_GPU_DESCRIPTOR_HANDLE& OutGpuHandle, CD3DX12_CPU_DESCRIPTOR_HANDLE& OutCpuHandle);

		void Reset();

	private:
		void CreateCacheCbvSrvUavDescriptorHeap();
		void CreateCacheRtvDescriptorHeap();

		void ResetCacheCbvSrvUavDescriptorHeap();
		void ResetCacheRtvDescriptorHeap();

	private:
		RenderDevice* m_Device = nullptr;

		// CbvSrvUav Descriptor Heap
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_CacheCbvSrvUavDescriptorHeap = nullptr;
		UINT m_CbvSrvUavDescriptorSize;
		static const int m_MaxCbvSrvUavDescripotrCount = 2048;
		uint32_t m_CbvSrvUavDescriptorOffset = 0;

		// RTV Descriptor Heap
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_CacheRtvDescriptorHeap = nullptr;
		UINT m_RtvDescriptorSize;
		static const int m_MaxRtvDescriptorCount = 1024;
		uint32_t m_RtvDescriptorOffset = 0;
	};

}