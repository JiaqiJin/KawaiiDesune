#pragma once

#include "D3D12Device.h"

namespace RHI
{
	class D3D12GraphicRHI
	{
	public:
		D3D12GraphicRHI();
		~D3D12GraphicRHI();

		void Initialize(HWND WindowHandle, int WindowWidth, int WindowHeight);
	private:
		std::unique_ptr<RenderDevice> m_RenderDevice = nullptr;
	};
}