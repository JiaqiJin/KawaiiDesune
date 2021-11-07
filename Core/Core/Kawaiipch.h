#ifndef PCH_H
#define PCH_H

// Windows
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define NODRAWTEXT
#define NOGDI
#define NOMCX
#define NOSERVICE
#define NOHELP


#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union
#pragma warning(disable:4238) // nonstandard extension used : class rvalue used as lvalue
#pragma warning(disable:4239) // A non-const reference may only be bound to an lvalue; assignment operator takes a reference to non-const
#pragma warning(disable:4324) // structure was padded due to __declspec(align())

#include <winsdkver.h>
#define _WIN32_WINNT 0x0A00
#include <sdkddkver.h>

// STL
#include <algorithm>
#include <array>
#include <atomic>
#include <execution>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <map>
#include <stdexcept>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <assert.h>
#include <ppltasks.h>

// Direct3d12
#include <D3d12SDKLayers.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <pix3.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

#include <wrl/client.h>
#include <wrl/event.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#include <windows.h>
#include <WindowsX.h>
#include <wrl.h>
#include <pathcch.h>
#include <shellapi.h>
#include <intrin.h>
#include <xmmintrin.h>

#define D3D12_GPU_VIRTUAL_ADDRESS_NULL      ((D3D12_GPU_VIRTUAL_ADDRESS)0)
#define D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN   ((D3D12_GPU_VIRTUAL_ADDRESS)-1)

#include "Math//VectorMath.h"
#include "Math/Common.h"

#include "Utils/d3dx12.h"
#include "Utils/Debug.h"
#include "Utils/DxException.h"
#include "Utils/Singleton.h"

#endif