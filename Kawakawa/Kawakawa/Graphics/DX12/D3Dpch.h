#pragma once

#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union
#pragma warning(disable:4238) // nonstandard extension used : class rvalue used as lvalue
#pragma warning(disable:4239) // A non-const reference may only be bound to an lvalue; assignment operator takes a reference to non-const
#pragma warning(disable:4324) // structure was padded due to __declspec(align())

// Use the C++ standard templated min/max
#ifndef NOMINMAX
#define NOMINMAX
#endif

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN

// STL
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <functional>
#include <queue>
#include <mutex>
#include <array>

// DX12
#include <d3d12.h>
#include "DXHelper.h"
#include "d3dx12.h"
#include <dxgi1_6.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#undef near
#undef far

using namespace Microsoft::WRL;

#define D3D12_GPU_VIRTUAL_ADDRESS_NULL ((D3D12_GPU_VIRTUAL_ADDRESS)0)
#define D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN ((D3D12_GPU_VIRTUAL_ADDRESS)-1)
