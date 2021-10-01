#pragma once

#if defined(KAWAII_CONFIGURATION_DEBUG) || defined(KAWAII_CONFIGURATION_DEVELOPMENT)
#define KAWAII_DEBUG
#else
#define KAWAII_DEBUG
// No debug
#endif

#define KAWAII_ENABLE_STATS

#define KAWAII_GAME_NAME L"Test Game"
#define KAWAII_ENGINE_NAME L"Boolka Engine"
#define KAWAII_IN_FLIGHT_FRAMES 2

#define KAWAII_FILE_BLOCK_SIZE 4096

// 126 is not a typo, it's reccomendation by nvidia
// https://developer.nvidia.com/blog/introduction-turing-mesh-shaders/
#define KAWAII_MESHLET_MAX_VERTS 64
#define KAWAII_MESHLET_MAX_PRIMS 126

// Enables usage of SSE intrinsics
#define KAWAII_USE_SSE

// ------------------------------------------------------------------


#if defined(KAWAII_CONFIGURATION_DEBUG)
#define KAWAII_RENDER_DEBUG
#define KAWAII_RENDER_DEVICE_LOST_DEBUG
#define KAWAII_RENDER_PROFILING
//#define KAWAII_USE_GPU_VALIDATION
#elif defined(KAWAII_CONFIGURATION_DEVELOPMENT)
#define KAWAII_RENDER_DEVICE_LOST_DEBUG
#define KAWAII_RENDER_PROFILING
#else
#define KAWAII_RENDER_DEVICE_LOST_DEBUG
#endif

#ifdef KAWAII_RENDER_PROFILING
#define KAWAII_USE_PIX_MARKERS
#endif

#ifdef KAWAII_USE_PIX_MARKERS
#define USE_PIX
#endif

// Concatenates KAWAII_ENGINE_NAME which is a string with " Window Class"
#define KAWAII_WINDOW_CLASS_NAME (KAWAII_ENGINE_NAME L" Window Class")

#define KAWAII_D3D12_SEMANTIC_MAX_LENGTH 32

#define KAWAII_MAX_LIGHT_COUNT 4
#define KAWAII_SUN_SHADOWMAP_SIZE 8192
#define KAWAII_LIGHT_SHADOWMAP_SIZE 1024

#define KAWAII_USE_COMMON_STATE_PROMOTION
