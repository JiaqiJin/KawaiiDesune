#pragma once

#define D3D12MA_D3D12_HEADERS_ALREADY_INCLUDED
#include "D3D12MemAlloc.h"

using namespace Microsoft::WRL;

namespace RHI
{
	class GraphicCore
	{
	public:
		GraphicCore(void* window_handle);
		GraphicCore(GraphicCore const&) = delete;
		GraphicCore(GraphicCore&&) = default;
		~GraphicCore();

	private:
		static constexpr UINT BACKBUFFER_COUNT = 3;

		UINT width, height;

		ComPtr<ID3D12Device5> m_device;
		ComPtr<IDXGISwapChain3> m_swapChain;

		// Commands
		ComPtr<ID3D12CommandQueue> m_graphicQueue;

		ComPtr<ID3D12CommandAllocator> m_cmdAllocator;
		// Sync
		ComPtr<ID3D12Fence> m_frameFence[BACKBUFFER_COUNT];
		HANDLE m_frameFenceHandle[BACKBUFFER_COUNT];

	};
}