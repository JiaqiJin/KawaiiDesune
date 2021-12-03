#pragma once

#include "RootSignature.h"

namespace Kawaii::Graphics::backend::DX12
{
	class PSO : public backend::Resource
	{
	public:
		PSO(std::string_view name) : m_Name(name) {}
		PSO(const PSO&) = delete;
		PSO& operator=(const PSO&) = delete;
		PSO(PSO&&) = default;
		PSO& operator=(PSO&&) = default;

		void SetRootSignature(const RootSignature& rootSig) { m_RootSignature = &rootSig; }
		ID3D12PipelineState* GetPSO() const { return m_PSO.Get(); }
	protected:
		std::string m_Name;
		const RootSignature* m_RootSignature = nullptr;
		ComPtr<ID3D12PipelineState> m_PSO;
	};

	class GraphicsPSO : public PSO
	{
	public:
		using PSO::PSO;

		void SetInputLayout(const std::vector<D3D12_INPUT_ELEMENT_DESC>& input_layout);
		void SetVertexShader(CD3DX12_SHADER_BYTECODE code);
		void SetPixelShader(CD3DX12_SHADER_BYTECODE code);
		void SetBlendState(const D3D12_BLEND_DESC& desc);
		void SetRasterizerState(const D3D12_RASTERIZER_DESC& desc);
		void SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& desc);
		void SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE type);
		void SetSampleMask(unsigned mask);
		void SetRenderTargetFormats(const std::vector<DXGI_FORMAT>& rtv_formats, DXGI_FORMAT dsv_format, unsigned msaa_count = 1, unsigned msaa_quality = 0);

		void Finalize(ID3D12Device* device);

		GraphicsPSO Copy() const;
	private:
		D3D12_GRAPHICS_PIPELINE_STATE_DESC m_PSODesc{};
		std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayouts;
	};

	class ComputePSO : public PSO
	{
	public:
		ComputePSO(std::string_view name);
	private:
		D3D12_COMPUTE_PIPELINE_STATE_DESC m_PSODesc{};
	};
}