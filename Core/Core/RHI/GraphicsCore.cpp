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

		m_RTVHeap = std::make_unique<DescriptorHeap>(m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 
			D3D12_DESCRIPTOR_HEAP_FLAG_NONE, BACKBUFFER_COUNT);

		// Frame Resouce
		{
			for (UINT fr = 0; fr < BACKBUFFER_COUNT; fr++)
			{
				ThrowIfFailed(m_swapChain->GetBuffer(fr, IID_PPV_ARGS(&frames[fr].m_backBuffer)));
				frames[fr].m_backBufferRTV = m_RTVHeap->GetCpuHandle(fr);
				m_device->CreateRenderTargetView(frames[fr].m_backBuffer.Get(), nullptr, frames[fr].m_backBufferRTV);

				ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
					IID_PPV_ARGS(frames[fr].m_default_cmd_Allocator.GetAddressOf())));

				ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, 
					frames[fr].m_default_cmd_Allocator.Get(),
					nullptr, IID_PPV_ARGS(frames[fr].m_default_cmd_List.GetAddressOf())));

				ThrowIfFailed(frames[fr].m_default_cmd_List->Close());

				for (UINT i = 0; i < CMD_LIST_COUNT; ++i)
				{
					// Direct Commands
					ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
						IID_PPV_ARGS(frames[fr].m_cmd_allocators[i].GetAddressOf())));

					ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, 
						frames[fr].m_cmd_allocators[i].Get(),
						nullptr, IID_PPV_ARGS(frames[fr].m_cmd_lists[i].GetAddressOf())));

					ThrowIfFailed(frames[fr].m_cmd_lists[i]->Close());

					// Compute Commands
					ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE,
						IID_PPV_ARGS(frames[fr].m_compute_cmd_llocators[i].GetAddressOf())));

					ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COMPUTE, 
						frames[fr].m_compute_cmd_llocators[i].Get(),
						nullptr, IID_PPV_ARGS(frames[fr].m_compute_cmd_lists[i].GetAddressOf())));

					ThrowIfFailed(frames[fr].m_compute_cmd_lists[i]->Close());
				}
			}
		}

		//sync objects
		{
			for (size_t i = 0; i < BACKBUFFER_COUNT; ++i)
			{
				ThrowIfFailed(m_device->CreateFence(m_frameFenceValues[i], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_frameFence[i])));
				m_frameFenceEvent[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
				if (m_frameFenceEvent == nullptr)
				{
					LOG_ERROR("Failed to create Frame Fence Event");
				}

				ThrowIfFailed(m_device->CreateFence(m_graphicsFenceValues[i], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_graphicsFences[i])));
				m_graphicsFenceEvents[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
				if (m_graphicsFenceEvents[i] == nullptr)
				{
					LOG_ERROR("Failed to create Graphic Fence Event");
				}

				ThrowIfFailed(m_device->CreateFence(m_computeFenceValues[i], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_computFences[i])));
				m_computeFenceEvents[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
				if (m_computeFenceEvents[i] == nullptr)
				{
					LOG_ERROR("Failed to create Compute Fence Event");
				}
			}
			
			m_waitFenceValue = 0;
			ThrowIfFailed(m_device->CreateFence(m_waitFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_waitFence)));
			m_waitFenceValue++;
			m_waitEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			if (m_waitEvent == nullptr) { LOG_ERROR("Failed to create Wait Fence Event"); }
		}
	}

	GraphicCore::~GraphicCore()
	{
		WaitForGPU();
		ProcessReleaseQueue();
		for (UINT i = 0; i < BACKBUFFER_COUNT; i++)
		{
			// Graphic
			if (m_graphicsFences[i]->GetCompletedValue() < m_graphicsFenceValues[i])
			{
				(m_graphicsFences[i]->SetEventOnCompletion(m_graphicsFenceValues[i], m_graphicsFenceEvents[i]));
				WaitForSingleObject(m_graphicsFenceEvents[i], INFINITE);
			}
			//Compute 
			if (m_graphicsFences[i]->GetCompletedValue() < m_graphicsFenceValues[i])
			{
				(m_computFences[i]->SetEventOnCompletion(m_computeFenceValues[i], m_computeFenceEvents[i]));
				WaitForSingleObject(m_computeFenceEvents[i], INFINITE);
			}

			CloseHandle(m_computeFenceEvents[i]);
			CloseHandle(m_graphicsFenceEvents[i]);
			CloseHandle(m_frameFenceEvent[i]);
		}
	}

	void GraphicCore::WaitForGPU()
	{
		// Signa command in Queue
		ThrowIfFailed(m_graphicQueue->Signal(m_waitFence.Get(), m_waitFenceValue));
		ThrowIfFailed(m_waitFence->SetEventOnCompletion(m_waitFenceValue, m_waitEvent));
		WaitForSingleObject(m_waitEvent, INFINITE);

		m_waitFenceValue++;
	}

	void GraphicCore::WaitOnQueue(QueueType type)
	{
		switch (type)
		{
		case RHI::QueueType::eGraphics:
			m_graphicQueue->Wait(m_computFences[m_backBufferIndex].Get(), m_computeFenceValues[m_backBufferIndex]);
			++m_computeFenceValues[m_backBufferIndex];
			break;
		case RHI::QueueType::eCompute:
			m_computeQueue->Wait(m_graphicsFences[m_backBufferIndex].Get(), m_graphicsFenceValues[m_backBufferIndex]);
			++m_graphicsFenceValues[m_backBufferIndex];
			break;
		}
	}

	void GraphicCore::SignalQueue(QueueType type)
	{
		switch (type)
		{
		case RHI::QueueType::eGraphics:
			m_graphicQueue->Signal(m_graphicsFences[m_backBufferIndex].Get(), m_graphicsFenceValues[m_backBufferIndex]);
			break;
		case RHI::QueueType::eCompute:
			m_computeQueue->Signal(m_computFences[m_backBufferIndex].Get(), m_computeFenceValues[m_backBufferIndex]);
			break;
		}
	}

	ID3D12GraphicsCommandList4* GraphicCore::GetDefaultCommandList() const
	{
		return GetFrameResources().m_default_cmd_List.Get();
	}

	ID3D12GraphicsCommandList4* GraphicCore::GetNewGraphicsCommandList() const
	{
		auto& frameResouce = GetFrameResources();

		unsigned int i = frameResouce.cmd_list_index;

		frameResouce.cmd_list_index++;

		assert(i < CMD_LIST_COUNT && "Not enough command lists");

		ThrowIfFailed(frameResouce.m_cmd_allocators[i]->Reset());
		ThrowIfFailed(frameResouce.m_cmd_lists[i]->Reset(frameResouce.m_cmd_allocators[i].Get(), nullptr));

		//ID3D12DescriptorHeap** ppHeaps[] = {}
		// TODO

		return frameResouce.m_cmd_lists[i].Get();
	}

	ID3D12GraphicsCommandList4* GraphicCore::GetLastGraphicsCommandList() const
	{
		auto& frameResouce = GetFrameResources();
		unsigned int i = frameResouce.cmd_list_index;
		return i > 0 ? frameResouce.m_cmd_lists[i - 1].Get() : frameResouce.m_default_cmd_List.Get();
	}

	ID3D12GraphicsCommandList4* GraphicCore::GetNewComputeCommandList() const
	{
		auto& frameResouce = GetFrameResources();

		unsigned int i = frameResouce.compute_cmd_list_index;

		frameResouce.compute_cmd_list_index++;

		assert(i < CMD_LIST_COUNT && "Not enough command lists");

		ThrowIfFailed(frameResouce.m_compute_cmd_llocators[i]->Reset());
		ThrowIfFailed(frameResouce.m_compute_cmd_lists[i]->Reset(frameResouce.m_compute_cmd_llocators[i].Get(), nullptr));

		// TODO

		return frameResouce.m_compute_cmd_lists[i].Get();
	}

	ID3D12GraphicsCommandList4* GraphicCore::GetLastComputeCommandList() const
	{
		auto& frameResouce = GetFrameResources();
		unsigned int i = frameResouce.compute_cmd_list_index;
		return i > 0 ? frameResouce.m_compute_cmd_lists[i - 1].Get() : frameResouce.m_default_cmd_List.Get();
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

	void GraphicCore::ProcessReleaseQueue()
	{

	}
}