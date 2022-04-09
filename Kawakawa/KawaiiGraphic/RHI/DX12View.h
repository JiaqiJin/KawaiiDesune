#pragma once

#include "DX12HeapSlotAllocator.h"

namespace RHI
{
	class RenderDevice;

	// Base class of all resource view
	class DX12View
	{
	public:
		DX12View(RenderDevice* device, D3D12_DESCRIPTOR_HEAP_TYPE type, ID3D12Resource* resource);
		virtual ~DX12View();

		D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandle() const { return m_HeapSlot.Handle; }

	private:
		void Destroy();

	protected:
		RenderDevice* m_Device = nullptr;
		// Resource
		ID3D12Resource* m_Resource = nullptr;
		// Heap Slot Allocator
		DX12HeapSlotAllocator* m_HeapSlotAllocator = nullptr;
		// Heap Slot
		DX12HeapSlotAllocator::HeapSlot m_HeapSlot;
		// Descriptor Heap Type
		D3D12_DESCRIPTOR_HEAP_TYPE m_Type;
	};

	// Shader Resource View
	class DX12ShaderResourceView : public DX12View
	{
	public:
		DX12ShaderResourceView(RenderDevice* device, const D3D12_SHADER_RESOURCE_VIEW_DESC& desc, ID3D12Resource* resource);
		virtual ~DX12ShaderResourceView();

	protected:
		void CreateShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& desc);
	};

	// Render Target View
	class DX12RenderTargetView : public DX12View
	{
	public:
		DX12RenderTargetView(RenderDevice* device, const D3D12_RENDER_TARGET_VIEW_DESC& desc, ID3D12Resource* resource);
		virtual ~DX12RenderTargetView();

	protected:
		void CreateRenderTargetView(const D3D12_RENDER_TARGET_VIEW_DESC& desc);
	};

	// Depth Stencil View
	class DX12DepthStencilView : public DX12View
	{
	public:
		DX12DepthStencilView(RenderDevice* device, const D3D12_DEPTH_STENCIL_VIEW_DESC& desc, ID3D12Resource* resource);

		virtual ~DX12DepthStencilView();

	protected:
		void CreateDepthStencilView(const D3D12_DEPTH_STENCIL_VIEW_DESC& desc);
	};

	// UnorderedAccessView
	class DX12UnorderedAccessView : public DX12View
	{
	public:
		DX12UnorderedAccessView(RenderDevice* device, const D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, ID3D12Resource* resource);

		virtual ~DX12UnorderedAccessView();

	protected:
		void CreateUnorderedAccessView(const D3D12_UNORDERED_ACCESS_VIEW_DESC& desc);
	};
}