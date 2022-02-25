#pragma once
#include <memory>
#include <string>
#include "IModule.h"
#include "../Core/Math/KawaiiMath.h"
#include "assimp/scene.h"


namespace Excalibur
{
	class IRenderResource
	{
	};

	enum VertexFormat
	{
		VF_None = 0,
		VF_P3F,
		VF_T2F,
		VF_N3F,
		VF_C4B,
	};

	enum IndexFormat 
	{
		IF_None = 0,
		IF_UINT16,
		IF_UINT32,
	};

	enum PrimitiveType 
	{
		PT_POINT = 1,
		PT_LINE,
		PT_TRIANGLE,
	};

	struct ConstantBuffer
	{
		Matrix4f world;
		Matrix4f view;
		Matrix4f projection;
		Vector4f debugColor;
	};
}