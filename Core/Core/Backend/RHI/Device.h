#pragma once

namespace Kawaii
{
	class Device
	{
	public:
		Device();
		~Device();

		bool Initialize();

		ID3D12Device6* GetD3D12Device() { return m_Device.Get(); }
	private:
		Microsoft::WRL::ComPtr<ID3D12Device6> m_Device;
	};
}