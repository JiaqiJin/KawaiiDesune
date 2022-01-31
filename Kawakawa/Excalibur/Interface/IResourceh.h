#pragma once

#include "../Core/Math/KawaiiMath.h"

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
		Matrix4x4f	world;
		Matrix4x4f	view;
		Matrix4x4f	projection;
		Vector4f	debugColor;
		Vector4f	pbrParameter;
		Vector4f	camPos;
	};

	struct ConstantBufferLighting 
	{
		Vector4f	SunLightDir;
		Vector4f	SunLightColor;

		Vector4f	PointLightDir[4];
		Vector4f	PointLightColor[4];
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
		VF_BONE_IDX_4I,
		VF_BONE_WEIGHT_4F,
	};

	enum IndexFormat
	{
		IF_None = 0,
		IF_UINT16,
		IF_UINT32,
	};
}