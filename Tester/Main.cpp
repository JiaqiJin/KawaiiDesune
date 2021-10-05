//#include <SDKDDKVer.h>
//#define WIN32_LEAN_AND_MEAN // 从 Windows 头中排除极少使用的资料
//#include <windows.h>
//#include <tchar.h>
////添加WTL支持 方便使用COM
//#include <wrl.h>
//using namespace Microsoft;
//using namespace Microsoft::WRL;
//
//#include <dxgi1_6.h>
//
//#include <DirectXMath.h>
//using namespace DirectX;
////for d3d12
//#include <d3d12.h>
//#include <d3d12shader.h>
//#include <d3dcompiler.h>
//
//#if defined(_DEBUG)
//#include <dxgidebug.h>
//#endif
//
//#include "Common/d3dx12.h"
//
//#define GRS_WND_CLASS_NAME _T("Game Window Class")
//#define GRS_WND_TITLE	_T("DirectX12 Trigger Sample")
//
//#define GRS_THROW_IF_FAILED(hr) if (FAILED(hr)){ throw CGRSCOMException(hr); }
//
//class CGRSCOMException
//{
//public:
//	CGRSCOMException(HRESULT hr) : m_hrError(hr)
//	{
//	}
//	HRESULT Error() const
//	{
//		return m_hrError;
//	}
//private:
//	const HRESULT m_hrError;
//};
//
//struct GRS_VERTEX
//{
//	XMFLOAT3 position;
//	XMFLOAT4 color;
//};
//
//LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//
//int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR    lpCmdLine, int nCmdShow)
//{
//	const UINT nFrameBackBufCount = 3u;
//
//	int iWidth = 1024;
//	int iHeight = 768;
//	UINT nFrameIndex = 0;
//	UINT nFrame = 0;
//
//	UINT nDXGIFactoryFlags = 0U;
//	UINT nRTVDescriptorSize = 0U;
//
//	HWND hWnd = nullptr;
//	MSG	msg = {};
//
//	float fAspectRatio = 3.0f;
//
//	D3D12_VERTEX_BUFFER_VIEW stVertexBufferView = {};
//
//	UINT64 n64FenceValue = 0ui64;
//	HANDLE hFenceEvent = nullptr;
//
//
//	CD3DX12_VIEWPORT stViewPort(0.0f, 0.0f, static_cast<float>(iWidth), static_cast<float>(iHeight));
//	CD3DX12_RECT	 stScissorRect(0, 0, static_cast<LONG>(iWidth), static_cast<LONG>(iHeight));
//
//	ComPtr<IDXGIFactory5>				pIDXGIFactory5;
//	ComPtr<IDXGIAdapter1>				pIAdapter;
//	ComPtr<ID3D12Device4>				pID3DDevice;
//	ComPtr<ID3D12CommandQueue>			pICommandQueue;
//	ComPtr<IDXGISwapChain1>				pISwapChain1;
//	ComPtr<IDXGISwapChain3>				pISwapChain3;
//	ComPtr<ID3D12DescriptorHeap>		pIRTVHeap;
//	ComPtr<ID3D12Resource>				pIARenderTargets[nFrameBackBufCount];
//	ComPtr<ID3D12CommandAllocator>		pICommandAllocator;
//	ComPtr<ID3D12RootSignature>			pIRootSignature;
//	ComPtr<ID3D12PipelineState>			pIPipelineState;
//	ComPtr<ID3D12GraphicsCommandList>	pICommandList;
//	ComPtr<ID3D12Resource>				pIVertexBuffer;
//	ComPtr<ID3D12Fence>					pIFence;
//
//	try
//	{
//
//		WNDCLASSEX wcex = {};
//		wcex.cbSize = sizeof(WNDCLASSEX);
//		wcex.style = CS_GLOBALCLASS;
//		wcex.lpfnWndProc = WndProc;
//		wcex.cbClsExtra = 0;
//		wcex.cbWndExtra = 0;
//		wcex.hInstance = hInstance;
//		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
//		wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);		//防止无聊的背景重绘
//		wcex.lpszClassName = GRS_WND_CLASS_NAME;
//		RegisterClassEx(&wcex);
//
//		hWnd = CreateWindowW(GRS_WND_CLASS_NAME
//			, GRS_WND_TITLE
//			, WS_OVERLAPPED | WS_SYSMENU
//			, CW_USEDEFAULT
//			, 0
//			, iWidth
//			, iHeight
//			, nullptr
//			, nullptr
//			, hInstance
//			, nullptr);
//
//		if (!hWnd)
//		{
//			return FALSE;
//		}
//
//		ShowWindow(hWnd, nCmdShow);
//		UpdateWindow(hWnd);
//
//#if defined(_DEBUG)
//		{//打开显示子系统的调试支持
//			ComPtr<ID3D12Debug> debugController;
//			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
//			{
//				debugController->EnableDebugLayer();
//				// 打开附加的调试支持
//				nDXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
//			}
//		}
//#endif
//		//1、创建DXGI Factory对象
//		GRS_THROW_IF_FAILED(CreateDXGIFactory2(nDXGIFactoryFlags, IID_PPV_ARGS(&pIDXGIFactory5)));
//		// 关闭ALT+ENTER键切换全屏的功能，因为我们没有实现OnSize处理，所以先关闭
//		GRS_THROW_IF_FAILED(pIDXGIFactory5->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));
//
//		//2、枚举适配器，并选择合适的适配器来创建3D设备对象
//		for (UINT adapterIndex = 1; DXGI_ERROR_NOT_FOUND != pIDXGIFactory5->EnumAdapters1(adapterIndex, &pIAdapter); ++adapterIndex)
//		{
//			DXGI_ADAPTER_DESC1 desc = {};
//			pIAdapter->GetDesc1(&desc);
//
//			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
//			{//跳过软件虚拟适配器设备
//				continue;
//			}
//			//检查适配器对D3D支持的兼容级别，这里直接要求支持12.1的能力，注意返回接口的那个参数被置为了nullptr，这样
//			//就不会实际创建一个设备了，也不用我们啰嗦的再调用release来释放接口。这也是一个重要的技巧，请记住！
//			if (SUCCEEDED(D3D12CreateDevice(pIAdapter.Get(), D3D_FEATURE_LEVEL_12_1, _uuidof(ID3D12Device), nullptr)))
//			{
//				break;
//			}
//		}
//		//3、创建D3D12.1的设备
//		GRS_THROW_IF_FAILED(D3D12CreateDevice(pIAdapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&pID3DDevice)));
//
//		//4、创建直接命令队列
//		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
//		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
//		GRS_THROW_IF_FAILED(pID3DDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&pICommandQueue)));
//
//		//5、创建交换链
//		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
//		swapChainDesc.BufferCount = nFrameBackBufCount;
//		swapChainDesc.Width = iWidth;
//		swapChainDesc.Height = iHeight;
//		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
//		swapChainDesc.SampleDesc.Count = 1;
//
//		GRS_THROW_IF_FAILED(pIDXGIFactory5->CreateSwapChainForHwnd(
//			pICommandQueue.Get(),		// Swap chain needs the queue so that it can force a flush on it.
//			hWnd,
//			&swapChainDesc,
//			nullptr,
//			nullptr,
//			&pISwapChain1
//		));
//
//		GRS_THROW_IF_FAILED(pISwapChain1.As(&pISwapChain3));
//		//6、得到当前后缓冲区的序号，也就是下一个将要呈送显示的缓冲区的序号
//		//注意此处使用了高版本的SwapChain接口的函数
//		nFrameIndex = pISwapChain3->GetCurrentBackBufferIndex();
//
//		//7、创建RTV(渲染目标视图)描述符堆(这里堆的含义应当理解为数组或者固定大小元素的固定大小显存池)
//		{
//			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
//			rtvHeapDesc.NumDescriptors = nFrameBackBufCount;
//			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
//			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
//
//			GRS_THROW_IF_FAILED(pID3DDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&pIRTVHeap)));
//			//得到每个描述符元素的大小
//			nRTVDescriptorSize = pID3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//		}
//
//		//8、创建RTV的描述符
//		{
//			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(pIRTVHeap->GetCPUDescriptorHandleForHeapStart());
//			for (UINT i = 0; i < nFrameBackBufCount; i++)
//			{
//				GRS_THROW_IF_FAILED(pISwapChain3->GetBuffer(i, IID_PPV_ARGS(&pIARenderTargets[i])));
//				pID3DDevice->CreateRenderTargetView(pIARenderTargets[i].Get(), nullptr, rtvHandle);
//				rtvHandle.Offset(1, nRTVDescriptorSize);
//			}
//		}
//
//		//9、创建一个空的根描述符，也就是默认的根描述符
//		{
//			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
//			rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
//
//			ComPtr<ID3DBlob> signature;
//			ComPtr<ID3DBlob> error;
//			GRS_THROW_IF_FAILED(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
//			GRS_THROW_IF_FAILED(pID3DDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&pIRootSignature)));
//		}
//
//		// 10、编译Shader创建渲染管线状态对象
//		{
//			ComPtr<ID3DBlob> vertexShader;
//			ComPtr<ID3DBlob> pixelShader;
//
//#if defined(_DEBUG)
//			// Enable better shader debugging with the graphics debugging tools.
//			UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
//#else
//			UINT compileFlags = 0;
//#endif
//			TCHAR pszShaderFileName[MAX_PATH] = _T("E:\VisualStudio\KawaiiEngine\Tester\Shader\\shaders.hlsl");
//
//			GRS_THROW_IF_FAILED(D3DCompileFromFile(pszShaderFileName, nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
//			GRS_THROW_IF_FAILED(D3DCompileFromFile(pszShaderFileName, nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));
//
//			// Define the vertex input layout.
//			D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
//			{
//				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
//				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
//			};
//
//			// Describe and create the graphics pipeline state object (PSO).
//			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
//			psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
//			psoDesc.pRootSignature = pIRootSignature.Get();
//			psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
//			psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
//			psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
//			psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
//			psoDesc.DepthStencilState.DepthEnable = FALSE;
//			psoDesc.DepthStencilState.StencilEnable = FALSE;
//			psoDesc.SampleMask = UINT_MAX;
//			psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
//			psoDesc.NumRenderTargets = 1;
//			psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
//			psoDesc.SampleDesc.Count = 1;
//
//			GRS_THROW_IF_FAILED(pID3DDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pIPipelineState)));
//		}
//
//		// 11、创建顶点缓冲
//		{
//			// 定义三角形的3D数据结构，每个顶点使用三原色之一
//			GRS_VERTEX triangleVertices[] =
//			{
//				{ { 0.0f, 0.25f * fAspectRatio, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
//				{ { 0.25f * fAspectRatio, -0.25f * fAspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
//				{ { -0.25f * fAspectRatio, -0.25f * fAspectRatio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
//			};
//
//			const UINT vertexBufferSize = sizeof(triangleVertices);
//
//			GRS_THROW_IF_FAILED(pID3DDevice->CreateCommittedResource(
//				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//				D3D12_HEAP_FLAG_NONE,
//				&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
//				D3D12_RESOURCE_STATE_GENERIC_READ,
//				nullptr,
//				IID_PPV_ARGS(&pIVertexBuffer)));
//
//			UINT8* pVertexDataBegin = nullptr;
//			CD3DX12_RANGE readRange(0, 0);		// We do not intend to read from this resource on the CPU.
//			GRS_THROW_IF_FAILED(pIVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
//			memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
//			pIVertexBuffer->Unmap(0, nullptr);
//
//			stVertexBufferView.BufferLocation = pIVertexBuffer->GetGPUVirtualAddress();
//			stVertexBufferView.StrideInBytes = sizeof(GRS_VERTEX);
//			stVertexBufferView.SizeInBytes = vertexBufferSize;
//		}
//
//		// 12、创建命令列表分配器
//		GRS_THROW_IF_FAILED(pID3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pICommandAllocator)));
//
//		// 13、创建图形命令列表
//		GRS_THROW_IF_FAILED(pID3DDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pICommandAllocator.Get(), pIPipelineState.Get(), IID_PPV_ARGS(&pICommandList)));
//
//		// 14、创建一个同步对象——围栏，用于等待渲染完成，因为现在Draw Call是异步的了
//		GRS_THROW_IF_FAILED(pID3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pIFence)));
//		n64FenceValue = 1;
//
//		// 15、创建一个Event同步对象，用于等待围栏事件通知
//		hFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
//		if (hFenceEvent == nullptr)
//		{
//			GRS_THROW_IF_FAILED(HRESULT_FROM_WIN32(GetLastError()));
//		}
//
//		//16、创建定时器对象，以便于创建高效的消息循环
//		HANDLE phWait = CreateWaitableTimer(NULL, FALSE, NULL);
//		LARGE_INTEGER liDueTime = {};
//		liDueTime.QuadPart = -1i64;//1秒后开始计时
//		SetWaitableTimer(phWait, &liDueTime, 1, NULL, NULL, 0);//40ms的周期
//
//		//17、开始消息循环，并在其中不断渲染
//		DWORD dwRet = 0;
//		BOOL bExit = FALSE;
//		while (!bExit)
//		{
//			dwRet = ::MsgWaitForMultipleObjects(1, &phWait, FALSE, INFINITE, QS_ALLINPUT);
//			switch (dwRet - WAIT_OBJECT_0)
//			{
//			case 0:
//			case WAIT_TIMEOUT:
//			{//计时器时间到
//				//GRS_TRACE(_T("开始第%u帧渲染{Frame Index = %u}：\n"),nFrame,nFrameIndex);
//				//开始记录命令
//				pICommandList->SetGraphicsRootSignature(pIRootSignature.Get());
//				pICommandList->RSSetViewports(1, &stViewPort);
//				pICommandList->RSSetScissorRects(1, &stScissorRect);
//
//				// 通过资源屏障判定后缓冲已经切换完毕可以开始渲染了
//				pICommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(pIARenderTargets[nFrameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
//
//				CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(pIRTVHeap->GetCPUDescriptorHandleForHeapStart(), nFrameIndex, nRTVDescriptorSize);
//				//设置渲染目标
//				pICommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
//
//				// 继续记录命令，并真正开始新一帧的渲染
//				const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
//				pICommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
//				pICommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//				pICommandList->IASetVertexBuffers(0, 1, &stVertexBufferView);
//
//				//Draw Call！！！
//				pICommandList->DrawInstanced(3, 1, 0, 0);
//
//				//又一个资源屏障，用于确定渲染已经结束可以提交画面去显示了
//				pICommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(pIARenderTargets[nFrameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
//				//关闭命令列表，可以去执行了
//				GRS_THROW_IF_FAILED(pICommandList->Close());
//
//				//执行命令列表
//				ID3D12CommandList* ppCommandLists[] = { pICommandList.Get() };
//				pICommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
//
//				//提交画面
//				GRS_THROW_IF_FAILED(pISwapChain3->Present(1, 0));
//
//				//开始同步GPU与CPU的执行，先记录围栏标记值
//				const UINT64 fence = n64FenceValue;
//				GRS_THROW_IF_FAILED(pICommandQueue->Signal(pIFence.Get(), fence));
//				n64FenceValue++;
//
//				// 看命令有没有真正执行到围栏标记的这里，没有就利用事件去等待，注意使用的是命令队列对象的指针
//				if (pIFence->GetCompletedValue() < fence)
//				{
//					GRS_THROW_IF_FAILED(pIFence->SetEventOnCompletion(fence, hFenceEvent));
//					WaitForSingleObject(hFenceEvent, INFINITE);
//				}
//				//执行到这里说明一个命令队列完整的执行完了，在这里就代表我们的一帧已经渲染完了，接着准备执行下一帧渲染
//
//				//获取新的后缓冲序号，因为Present真正完成时后缓冲的序号就更新了
//				nFrameIndex = pISwapChain3->GetCurrentBackBufferIndex();
//
//				//命令分配器先Reset一下
//				GRS_THROW_IF_FAILED(pICommandAllocator->Reset());
//				//Reset命令列表，并重新指定命令分配器和PSO对象
//				GRS_THROW_IF_FAILED(pICommandList->Reset(pICommandAllocator.Get(), pIPipelineState.Get()));
//
//				//GRS_TRACE(_T("第%u帧渲染结束.\n"), nFrame++);
//			}
//			break;
//			case 1:
//			{//处理消息
//				while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//				{
//					if (WM_QUIT != msg.message)
//					{
//						::TranslateMessage(&msg);
//						::DispatchMessage(&msg);
//					}
//					else
//					{
//						bExit = TRUE;
//					}
//				}
//			}
//			break;
//			default:
//				break;
//			}
//		}
//	}
//	catch (CGRSCOMException& e)
//	{//发生了COM异常
//		e;
//	}
//	return 0;
//}
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message)
//	{
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProc(hWnd, message, wParam, lParam);
//	}
//	return 0;
//}