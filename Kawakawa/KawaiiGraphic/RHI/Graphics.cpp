#include "pch.h"
#include "Graphics.h"

namespace RHI
{
	using Microsoft::WRL::ComPtr;

	// managers
	ID3D12Device* Graphics::s_Device = nullptr;
}