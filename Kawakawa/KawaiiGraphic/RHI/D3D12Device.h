#pragma once

namespace RHI
{
	class RenderDevice
	{
	public:
		RenderDevice();
		~RenderDevice();

		ID3D12Device* GetD3DDevice() { return m_Device.Get(); }

	private:
		void Initialize();

	private:
		Microsoft::WRL::ComPtr<ID3D12Device> m_Device = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory4> DxgiFactory = nullptr;
		// TODO
		// Upload Buffer
		// RTV, DSV, SRV Descriptor heap
	};
}