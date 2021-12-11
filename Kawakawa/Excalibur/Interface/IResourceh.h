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
}