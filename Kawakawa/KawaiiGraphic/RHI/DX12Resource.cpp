#include "pch.h"
#include "DX12Resource.h"


namespace RHI
{
	DX12Resource::DX12Resource(Microsoft::WRL::ComPtr<ID3D12Resource> Resource, D3D12_RESOURCE_STATES InitState)
		: m_Resource(Resource), m_CurrentState(InitState)
	{
		if (m_Resource->GetDesc().Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
		{
			m_GPUVirtualAddress = m_Resource->GetGPUVirtualAddress();
		}
	}

	void* DX12Resource::Map()
	{
		void* m_MappedAdderessMemory = nullptr;
		ThrowIfFailed(m_Resource->Map(0, nullptr, &m_MappedAdderessMemory));
		return m_MappedAdderessMemory;
	}
}