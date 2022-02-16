#pragma once

#include "../Core/Math/EigenMath.h"

#include <stdint.h>

namespace Excalibur
{
	class IResouceBase
	{
	public:
		//virtual ~IResouceBase() = 0;
	};

	struct ConstantBuffer
	{
		Matrix4f world;
		Matrix4f view;
		Matrix4f projection;
		Vector4f debugColor;
	};

	// Enum
	enum PrimitiveType 
	{
		PT_POINT = 1,
		PT_LINE,
		PT_TRIANGLE,
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
}