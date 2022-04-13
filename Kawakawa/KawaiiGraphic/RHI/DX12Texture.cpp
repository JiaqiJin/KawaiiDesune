#include "pch.h"
#include "DX12Texture.h"

namespace RHI
{
	DX12ShaderResourceView* DX12Texture::GetSRV(UINT Index)
	{
		assert(m_SRVs.size() > Index);
		return m_SRVs[Index].get();
	}

	void DX12Texture::AddSRV(std::unique_ptr<DX12ShaderResourceView>& SRV)
	{
		m_SRVs.emplace_back(std::move(SRV));
	}

	DX12RenderTargetView* DX12Texture::GetRTV(UINT Index)
	{
		assert(m_RTVs.size() > Index);
		return m_RTVs[Index].get();
	}

	void DX12Texture::AddRTV(std::unique_ptr<DX12RenderTargetView>& RTV)
	{
		m_RTVs.emplace_back(std::move(RTV));
	}

	DX12DepthStencilView* DX12Texture::GetDSV(UINT Index)
	{
		assert(m_DSVs.size() > Index);
		return m_DSVs[Index].get();
	}

	void DX12Texture::AddDSV(std::unique_ptr<DX12DepthStencilView>& DSV)
	{
		m_DSVs.emplace_back(std::move(DSV));
	}

	DX12UnorderedAccessView* DX12Texture::GetUAV(UINT Index)
	{
		assert(m_UAVs.size() > Index);
		return m_UAVs[Index].get();
	}

	void DX12Texture::AddUAV(std::unique_ptr<DX12UnorderedAccessView>& UAV)
	{
		m_UAVs.emplace_back(std::move(UAV));
	}
}