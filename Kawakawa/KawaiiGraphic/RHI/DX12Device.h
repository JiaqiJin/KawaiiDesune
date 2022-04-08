#pragma once

#include "DX12CommandContext.h"

namespace RHI
{
	class RenderDevice
	{
	public:
		RenderDevice();
		~RenderDevice();

		ID3D12Device* GetD3DDevice() { return m_Device.Get(); }
		DX12CommandContext* GetCommandContext() { return m_CommandContext.get(); }
		ID3D12CommandQueue* GetCommandQueue() { return m_CommandContext->GetCommandQueue(); }
		ID3D12GraphicsCommandList* GetCommandList() { return m_CommandContext->GetCommandList(); }

	private:
		void Initialize();

	private:
		Microsoft::WRL::ComPtr<ID3D12Device> m_Device = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory4> DxgiFactory = nullptr;

		std::unique_ptr<DX12CommandContext> m_CommandContext = nullptr;
		// TODO
		// Upload Buffer
		// RTV, DSV, SRV Descriptor heap
	};
}