#include "../Kawaiipch.h"

#include "GraphicsCore.h"

using namespace Microsoft::WRL;

namespace RHI
{
	GraphicCore::GraphicCore(void* window_handle)
	{
		HWND hwnd = static_cast<HWND>(window_handle);
		RECT rect{};
		GetClientRect(hwnd, &rect);
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;

		HRESULT hr = E_FAIL;
		UINT dxgi_factory_flags = 0;
#if defined(_DEBUG)
		Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
			dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
		}
#endif

		ComPtr<IDXGIFactory4> dxgiFactory;
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));
		
		ComPtr<IDXGIAdapter> warpAdapter;
		ThrowIfFailed(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

		ThrowIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device)));

#if defined(_DEBUG)
		if (debugController != NULL)
		{
			ID3D12InfoQueue* pInfoQueue = NULL;
			m_device->QueryInterface(IID_PPV_ARGS(&pInfoQueue));
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
			pInfoQueue->Release();
		}
#endif

		ComPtr<IDXGIAdapter1> pAdapter;
		dxgiFactory->EnumAdapters1(1, &pAdapter);

		// D3D12 Allocator
		{

		}

		// Create Command Queue
		{
			D3D12_COMMAND_QUEUE_DESC graphic_Queue_Desc = {};
			graphic_Queue_Desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			graphic_Queue_Desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			graphic_Queue_Desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			graphic_Queue_Desc.NodeMask = 0;
			ThrowIfFailed(m_device->CreateCommandQueue(&graphic_Queue_Desc, IID_PPV_ARGS(&m_graphicQueue)));
			m_graphicQueue->SetName(L"Graphic Queue");

			// Compute Queue
			D3D12_COMMAND_QUEUE_DESC compute_Queue_Desc = {};
			compute_Queue_Desc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
			compute_Queue_Desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			compute_Queue_Desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			compute_Queue_Desc.NodeMask = 0;
			ThrowIfFailed(m_device->CreateCommandQueue(&compute_Queue_Desc, IID_PPV_ARGS(&m_computeQueue)));
			m_computeQueue->SetName(L"Compute Queue");
		}

		// Create swap chain
		{
			IDXGISwapChain1* _swapChain = nullptr;
			DXGI_SWAP_CHAIN_DESC1 sd = {};
			sd.Width = width;
			sd.Height = height;
			sd.Format = DXGI_FORMAT_R10G10B10A2_UNORM;
			sd.Stereo = false;
			sd.SampleDesc.Count = 1; // Don't use multi-sampling.
			sd.SampleDesc.Quality = 0;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.BufferCount = BACKBUFFER_COUNT;
			sd.Flags = 0;
			sd.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
			sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
			sd.Scaling = DXGI_SCALING_NONE;
			ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(m_graphicQueue.Get(), hwnd, &sd, nullptr, nullptr, &_swapChain));
			ThrowIfFailed(_swapChain->QueryInterface(IID_PPV_ARGS(&m_swapChain)));

			m_backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
			m_Last_backBufferIndex = m_backBufferIndex;
		}

		//create upload and descriptor allocators
		{
			D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
			// TODO
		}



		//sync objects
		{
			for (size_t i = 0; i < BACKBUFFER_COUNT; ++i)
			{
				ThrowIfFailed(m_device->CreateFence(m_frameFenceValues[i], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_frameFence[i])));
				m_frameFenceEvent[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
				if (m_frameFenceEvent == nullptr)
				{
					// TODO
					// LOG
				}
			}
			
			m_waitFenceValue = 0;
			ThrowIfFailed(m_device->CreateFence(m_waitFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_waitFence)));
			m_waitFenceValue++;
			m_waitEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			if (m_waitEvent == nullptr) { /*log*/}
		}
	}

	GraphicCore::~GraphicCore()
	{
		// TODO
	}

	void GraphicCore::ResetDefaultCommandList()
	{
		auto& frameResouce = GetFrameResources();

		ThrowIfFailed(frameResouce.m_default_cmd_Allocator->Reset());
		ThrowIfFailed(frameResouce.m_default_cmd_List->Reset(frameResouce.m_default_cmd_Allocator.Get(), nullptr));
	}

	void GraphicCore::ExecuteDefaultCommandList()
	{
		auto& frameResouce = GetFrameResources();

		frameResouce.m_default_cmd_List->Close();

		ID3D12CommandList* cmd_List = frameResouce.m_default_cmd_List.Get();

		m_graphicQueue->ExecuteCommandLists(1, &cmd_List);
	}

	GraphicCore::FrameResources& GraphicCore::GetFrameResources()
	{
		return frames[m_backBufferIndex];
	}

	const GraphicCore::FrameResources& GraphicCore::GetFrameResources() const
	{
		return frames[m_backBufferIndex];
	}

	void GraphicCore::ExecuteGraphicsCommandLists()
	{
		auto& frameResouce = GetFrameResources();

		frameResouce.m_default_cmd_List->Close();

		std::vector<ID3D12CommandList*> cmd_Lists = { frameResouce.m_default_cmd_List.Get() };

		for (UINT i = 0; i < frameResouce.cmd_list_index; ++i)
		{
			frameResouce.m_cmd_lists[i]->Close();
			cmd_Lists.push_back(frameResouce.m_cmd_lists[i].Get());
		}

		m_graphicQueue->ExecuteCommandLists(static_cast<UINT>(cmd_Lists.size()), cmd_Lists.data());

		frameResouce.cmd_list_index = 0;
	}

	void GraphicCore::ExecuteComputeCommandLists()
	{
		auto& frameResouce = GetFrameResources();

		if (frameResouce.compute_cmd_list_index == 0) return;

		std::vector<ID3D12CommandList*> cmd_Lists = {};

		for (UINT i = 0; i < frameResouce.compute_cmd_list_index; ++i)
		{
			frameResouce.m_compute_cmd_lists[i]->Close();
			cmd_Lists.push_back(frameResouce.m_compute_cmd_lists[i].Get());
		}

		m_computeQueue->ExecuteCommandLists(static_cast<UINT>(cmd_Lists.size()), cmd_Lists.data());

		frameResouce.compute_cmd_list_index = 0;
	}
}