#pragma once

// 添加要在此处预编译的标头
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows
#include <windows.h>
#include <WindowsX.h>
#include <wrl.h>


// Direct3d12
#include <d3d12.h>
#include <D3d12SDKLayers.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

#if defined(_DEBUG)
#include <dxgidebug.h>
#endif

#include "Common/d3dx12.h"
#include "Common/Helpers.h"

// STL Headers
#include <array>
#include <vector>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include <string>
#include <exception>
#include <shellapi.h>
#include <cassert>
#include <iostream>
#include <limits>

// The min/max macros conflict with like-named member functions.
// Only use std::min and std::max defined in <algorithm>.
#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif

// In order to define a function called CreateWindow, the Windows macro needs to
// be undefined.
#if defined(CreateWindow)
#undef CreateWindow
#endif

// ---------------------- 
