#pragma once

#include "ImageParser.h"

namespace Kawaii::Asset
{
	class JpegParser : public ImageParser
	{
	public:
		Image Parse(const Core::Buffer& buf) final;
	};
}