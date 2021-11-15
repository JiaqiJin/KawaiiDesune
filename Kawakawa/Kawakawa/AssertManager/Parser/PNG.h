#pragma once

#include "ImageParser.h"

namespace Kawaii::Asset
{
	class PngParser : public ImageParser
	{
	public:
		Image Parse(const Core::Buffer& buf) final;
	};
}