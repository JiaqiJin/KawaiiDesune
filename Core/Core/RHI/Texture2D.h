#pragma once

namespace RHI
{
	struct Texture2D_Desc
	{
		uint32_t width;
		uint32_t height;
		DXGI_FORMAT format;
		D3D12_CLEAR_VALUE clear_value;
		uint32_t mips = 1;
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;
		D3D12_RESOURCE_STATES start_state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	};

	class Texture2D
	{
	public:
		Texture2D() = default;

		Texture2D(ID3D12Device* device, Texture2D_Desc const& desc) :m_device(device)
		{
			D3D12_RESOURCE_DESC resourceDesc = {};
			resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			resourceDesc.DepthOrArraySize = 1;
			resourceDesc.MipLevels = desc.mips;
			resourceDesc.Flags = desc.flags;
			resourceDesc.Format = desc.format;
			resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			resourceDesc.SampleDesc.Count = 1u;
			resourceDesc.SampleDesc.Quality = 0u;
			resourceDesc.Width = desc.width;
			resourceDesc.Height = desc.height;
			auto heap_type = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

			D3D12_CLEAR_VALUE const* clear = nullptr;

			if(desc.flags & (D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET | D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL))
				clear = &desc.clear_value;

			ThrowIfFailed(device->CreateCommittedResource(
				&heap_type,
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc,
				desc.start_state,
				clear,
				IID_PPV_ARGS(&m_resource)));
		}

		ID3D12Resource* Resource() const { return m_resource.Get(); }
		uint32_t Width() const{ return (uint32_t)m_resource->GetDesc().Width; }
		uint32_t Height() const { return (uint32_t)m_resource->GetDesc().Height; }
		DXGI_FORMAT Format() const { return m_resource->GetDesc().Format; }
	private:
		ID3D12Device* m_device;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_resource = nullptr;

	};
}