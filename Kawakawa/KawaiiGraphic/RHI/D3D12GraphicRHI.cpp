#include "pch.h"
#include "D3D12GraphicRHI.h"

using Microsoft::WRL::ComPtr;

namespace RHI
{
	D3D12GraphicRHI::D3D12GraphicRHI()
	{

	}

	D3D12GraphicRHI::~D3D12GraphicRHI()
	{

	}

	void D3D12GraphicRHI::Initialize(HWND WindowHandle, int WindowWidth, int WindowHeight)
	{
		m_RenderDevice = std::make_unique<RenderDevice>();
	}
}