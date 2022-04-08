#pragma once

namespace RHI
{
	class RenderDevice;

	class DX12GraphicRHI
	{
	public:
		DX12GraphicRHI();
		~DX12GraphicRHI();

		void Initialize(HWND WindowHandle, int WindowWidth, int WindowHeight);
	private:
		std::unique_ptr<RenderDevice> m_RenderDevice = nullptr;
	};
}