#pragma once

namespace RHI
{
	class RenderDevice;

	class DX12Resource
	{
	public:
		DX12Resource(Microsoft::WRL::ComPtr<ID3D12Resource> Resource, D3D12_RESOURCE_STATES InitState = D3D12_RESOURCE_STATE_COMMON);

		void* Map();
	public:
		Microsoft::WRL::ComPtr<ID3D12Resource> m_Resource = nullptr;
		D3D12_GPU_VIRTUAL_ADDRESS m_GPUVirtualAddress = 0;
		D3D12_RESOURCE_STATES m_CurrentState;		
	};
}