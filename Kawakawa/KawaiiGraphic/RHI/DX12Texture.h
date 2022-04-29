#pragma once

#include "DX12Resource.h"
#include "DX12View.h"
#include "../Math/Math.h"

namespace RHI
{
	class DX12Texture
	{
	public:
		DX12Resource* GetResource() { return m_ResourceAllocation.m_UnderlyingResource; }
		ID3D12Resource* GetD3DResource() { return m_ResourceAllocation.m_UnderlyingResource->m_Resource.Get(); }

		void SetRTVClearValue(TVector4 ClearValue) { RTVClearValue = ClearValue; }
		TVector4 GetRTVClearValue() { return RTVClearValue; }
		float* GetRTVClearValuePtr() { return (float*)&RTVClearValue; }

		// Getters
		DX12ShaderResourceView* GetSRV(UINT Index = 0);
		DX12RenderTargetView* GetRTV(UINT Index = 0);
		DX12DepthStencilView* GetDSV(UINT Index = 0);
		DX12UnorderedAccessView* GetUAV(UINT Index = 0);

		void AddSRV(std::unique_ptr<DX12ShaderResourceView>& SRV);
		void AddRTV(std::unique_ptr<DX12RenderTargetView>& RTV);
		void AddDSV(std::unique_ptr<DX12DepthStencilView>& DSV);
		void AddUAV(std::unique_ptr<DX12UnorderedAccessView>& UAV);
	public:
		DX12ResourceAllocation m_ResourceAllocation;
	private:
		// Resources Views
		std::vector<std::unique_ptr<DX12ShaderResourceView>> m_SRVs;
		std::vector<std::unique_ptr<DX12RenderTargetView>> m_RTVs;
		std::vector<std::unique_ptr<DX12DepthStencilView>> m_DSVs;
		std::vector<std::unique_ptr<DX12UnorderedAccessView>> m_UAVs;

		// Clear Value
		TVector4 RTVClearValue;
	};

	typedef std::shared_ptr<DX12Texture> DX12TextureRef;
}