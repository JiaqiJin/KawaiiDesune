#pragma once

#include <DirectXMath.h>

#ifndef DECLSPEC_ALIGN
#define DECLSPEC_ALIGN(x)   __declspec(align(x))
#endif

using namespace DirectX;

namespace RHI
{
	DECLSPEC_ALIGN(16) struct FrameCBuffer
	{
		XMMATRIX view;
		XMMATRIX projection;
	};

	DECLSPEC_ALIGN(16) struct ObjectCBuffer
	{
		XMMATRIX model;
	};

	DECLSPEC_ALIGN(16) struct MaterialCBuffer
	{
		XMFLOAT3 ambient;
		XMFLOAT3 diffuse;
		XMFLOAT3 specular;
	};

}