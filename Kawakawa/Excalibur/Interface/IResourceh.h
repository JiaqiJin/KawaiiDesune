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
		Matrix4x4f world;
		Matrix4x4f view;
		Matrix4x4f projection;
	};
}