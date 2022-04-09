#include "pch.h"
#include "DX12View.h"
#include "DX12Device.h"

namespace RHI
{
	DX12View::DX12View(RenderDevice* device, D3D12_DESCRIPTOR_HEAP_TYPE type, ID3D12Resource* resource)
		: m_Device(device), m_Type(type), m_Resource(resource)
	{
		m_HeapSlotAllocator = m_Device->GetHeapSlotAllocator(type);

		if (m_HeapSlotAllocator)
		{
			m_HeapSlot = m_HeapSlotAllocator->AllocateHeapSlot();
			assert(m_HeapSlot.Handle.ptr != 0);
		}
	}

	DX12View::~DX12View()
	{
		Destroy();
	}

	void DX12View::Destroy()
	{
		if (m_HeapSlotAllocator)
			m_HeapSlotAllocator->FreeHeapSlot(m_HeapSlot);
	}

	// Shader Resource View
	DX12ShaderResourceView::DX12ShaderResourceView(RenderDevice* device, const D3D12_SHADER_RESOURCE_VIEW_DESC& desc, ID3D12Resource* resource)
		: DX12View(device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, resource)
	{
		CreateShaderResourceView(desc);
	}

	DX12ShaderResourceView::~DX12ShaderResourceView()
	{

	}

	void DX12ShaderResourceView::CreateShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& desc)
	{
		m_Device->GetD3DDevice()->CreateShaderResourceView(m_Resource, &desc, m_HeapSlot.Handle);
	}

	// Render Target View
	DX12RenderTargetView::DX12RenderTargetView(RenderDevice* device, const D3D12_RENDER_TARGET_VIEW_DESC& desc, ID3D12Resource* resource)
		: DX12View(device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, resource)
	{
		CreateRenderTargetView(desc);
	}

	DX12RenderTargetView::~DX12RenderTargetView()
	{

	}

	void DX12RenderTargetView::CreateRenderTargetView(const D3D12_RENDER_TARGET_VIEW_DESC& desc)
	{
		m_Device->GetD3DDevice()->CreateRenderTargetView(m_Resource, &desc, m_HeapSlot.Handle);
	}

	// Depth Stencil View
	DX12DepthStencilView::DX12DepthStencilView(RenderDevice* device, const D3D12_DEPTH_STENCIL_VIEW_DESC& desc, ID3D12Resource* resource)
		: DX12View(device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, resource)
	{
		CreateDepthStencilView(desc);
	}

	DX12DepthStencilView::~DX12DepthStencilView()
	{

	}

	void DX12DepthStencilView::CreateDepthStencilView(const D3D12_DEPTH_STENCIL_VIEW_DESC& desc)
	{
		m_Device->GetD3DDevice()->CreateDepthStencilView(m_Resource, &desc, m_HeapSlot.Handle);
	}

	// UnorderedAccess View
	DX12UnorderedAccessView::DX12UnorderedAccessView(RenderDevice* device,
		const D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, ID3D12Resource* resource)
		: DX12View(device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, resource)
	{
		CreateUnorderedAccessView(desc);
	}

	DX12UnorderedAccessView::~DX12UnorderedAccessView()
	{

	}


	void DX12UnorderedAccessView::CreateUnorderedAccessView(const D3D12_UNORDERED_ACCESS_VIEW_DESC& desc)
	{
		m_Device->GetD3DDevice()->CreateUnorderedAccessView(m_Resource, nullptr, &desc, m_HeapSlot.Handle);
	}
}