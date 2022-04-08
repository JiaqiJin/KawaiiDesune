#pragma once

/*
* Use a CPU descriptor heap to temporarily store all descriptors,
* and use a GPU descriptor heap to collect the descriptors needed for each drawing.
* Before each drawing, fill the GPU descriptor heap with the descriptors needed for the current drawing
* https://zhuanlan.zhihu.com/p/129257338
* https://github.com/EpicGames/UnrealEngine/blob/c3caf7b6bf12ae4c8e09b606f10a09776b4d1f38/Engine/Source/Runtime/D3D12RHI/Private/D3D12DescriptorCache.h
*/

namespace RHI
{
	class RenderDevice;
	// Shader-visible descriptor heap
	class DX12DescriptorCache
	{
	public:
		DX12DescriptorCache(RenderDevice* Device);
		~DX12DescriptorCache();

		// Getters
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetCacheCbvSrvUavDescriptorHeap() { return m_CacheCbvSrvUavDescriptorHeap; }
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetCacheRtvDescriptorHeap() { return m_CacheRtvDescriptorHeap; }

		// Copying the descriptor from non-shader-visible descriptor heap to the shader-visible descriptor heap.
		CD3DX12_GPU_DESCRIPTOR_HANDLE AppendCbvSrvUavDescriptors(const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& SrcDescriptors);
		void AppendRtvDescriptors(const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& RtvDescriptors, 
			CD3DX12_GPU_DESCRIPTOR_HANDLE& OutGpuHandle, CD3DX12_CPU_DESCRIPTOR_HANDLE& OutCpuHandle);

		void Reset();

	private:
		// Create a descriptor heap (D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)
		void CreateCacheCbvSrvUavDescriptorHeap();
		void CreateCacheRtvDescriptorHeap();

		// At the end of each frame, the DescriptorOffset is reset, which is equivalent to emptying the descriptor heap.
		void ResetCacheCbvSrvUavDescriptorHeap();
		void ResetCacheRtvDescriptorHeap();

	private:
		RenderDevice* m_Device = nullptr;

		// CbvSrvUav Descriptor Heap
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_CacheCbvSrvUavDescriptorHeap = nullptr;
		// CbvSrvUav Descriptor Size
		UINT m_CbvSrvUavDescriptorSize;
		static const int m_MaxCbvSrvUavDescripotrCount = 2048;
		// CbvSrvUav Descriptor Offset
		uint32_t m_CbvSrvUavDescriptorOffset = 0;

		// RTV Descriptor Heap
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_CacheRtvDescriptorHeap = nullptr;
		// RTV Descriptor Size
		UINT m_RtvDescriptorSize;
		static const int m_MaxRtvDescriptorCount = 1024;
		// RTV Descriptor Offset
		uint32_t m_RtvDescriptorOffset = 0;
	};

}