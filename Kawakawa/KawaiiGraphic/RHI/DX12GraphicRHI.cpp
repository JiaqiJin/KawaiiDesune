#include "pch.h"
#include "DX12GraphicRHI.h"
#include "DX12Device.h"

using Microsoft::WRL::ComPtr;

namespace RHI
{
	DX12GraphicRHI::DX12GraphicRHI()
	{

	}

	DX12GraphicRHI::~DX12GraphicRHI()
	{

	}

	void DX12GraphicRHI::Initialize(HWND WindowHandle, int WindowWidth, int WindowHeight)
	{
		m_RenderDevice = std::make_unique<RenderDevice>();
	}
}