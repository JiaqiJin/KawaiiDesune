#pragma once

#include "../Image.h"

namespace Kawaii::Asset
{
	enum class ImageFormat : unsigned
	{
		PNG,
		JPEG,
		TGA,
		BMP,
		NUM_SUPPORT
	};

	class ImageParser
	{
	public:
		virtual Image Parse() = 0;
		virtual ~ImageParser() = default;
	};
}