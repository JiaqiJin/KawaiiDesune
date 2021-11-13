#pragma once

#include "D3Dpch.h"
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

		void Present(size_t frameIndex) final;
		void CreateSwapChain(uint32_t width, uint32_t height, unsigned frameCount, Format format, void* window) final;

		ID3D12Device* GetDevice() const noexcept { return m_Device.Get(); }
		CommandListManager& GetCmdMgr() noexcept { return m_CommandManager; }

		// Fence
		void WaitFence(uint64_t fenceValue) final;
		void IdleGPU() final;

	private:
		// Static method
		static DXGI_FORMAT ToDxgiFormat(Graphics::Format format) noexcept { return static_cast<DXGI_FORMAT>(format); }
		static Graphics::Format ToFormat(DXGI_FORMAT format) noexcept { return static_cast<Graphics::Format>(format); }

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