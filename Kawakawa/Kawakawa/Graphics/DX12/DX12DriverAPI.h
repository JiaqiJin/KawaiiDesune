#pragma once

#include "../DriverApi.h"
#include "CommandContext.h"
#include "CommandListManager.h"

namespace Kawaii::Graphics::backend::DX12
{
	class DX12DriverAPI : public DriverAPI
	{
	public:
		DX12DriverAPI();
		~DX12DriverAPI();

		ID3D12Device* GetDevice() const noexcept { return m_Device.Get(); }
		CommandListManager& GetCmdMgr() noexcept { return m_CommandManager; }
	private:
		// Device
		ComPtr<ID3D12Device6> m_Device;
		ComPtr<IDXGIFactory4> m_DxgiFactory;
		// Swap Chain
		ComPtr<IDXGISwapChain4> m_SwapChain;
		// Viewpor and Scissor
		D3D12_VIEWPORT m_Viewport;
		D3D12_RECT     m_ScissorRect;

		CommandListManager m_CommandManager;
	};
}