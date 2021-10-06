#pragma once

#define D3D12MA_D3D12_HEADERS_ALREADY_INCLUDED
#include "D3D12MemAlloc.h"

using namespace Microsoft::WRL;

namespace RHI
{
	class GraphicCore
	{
		static constexpr UINT CMD_LIST_COUNT = 8;
		static constexpr UINT BACKBUFFER_COUNT = 3;

		struct FrameResources
		{
			ComPtr<ID3D12Resource> m_backBuffer;
			
			ComPtr<ID3D12CommandAllocator> m_cmd_defaultAllocator;
			ComPtr<ID3D12GraphicsCommandList4> m_cmd_defaultList;

			ComPtr<ID3D12CommandAllocator>      m_cmd_allocators[CMD_LIST_COUNT] = {};
			ComPtr<ID3D12GraphicsCommandList4>  m_cmd_lists[CMD_LIST_COUNT] = {};
		};

	public:
		GraphicCore(void* window_handle);
		GraphicCore(GraphicCore const&) = delete;
		GraphicCore(GraphicCore&&) = default;
		~GraphicCore();

		ID3D12Device5* GetDevice() const { return m_device.Get(); }

	private:

		UINT width, height;
		UINT m_backBufferIndex;
		UINT m_Last_backBufferIndex;

		ComPtr<ID3D12Device5> m_device;
		ComPtr<IDXGISwapChain3> m_swapChain;

		// Frame Resources
		FrameResources frames[BACKBUFFER_COUNT];

		// Commands Queue
		ComPtr<ID3D12CommandQueue> m_graphicQueue;

		// Sync
		Microsoft::WRL::ComPtr<ID3D12Fence> m_frameFence[BACKBUFFER_COUNT];
		HANDLE m_frameFenceEvent[BACKBUFFER_COUNT];
		UINT64 m_frameFenceValues[BACKBUFFER_COUNT];
		UINT64 m_frameFenceValue;
	};
}