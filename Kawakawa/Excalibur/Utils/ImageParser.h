#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <string>
#include "stb_image.h"

namespace Excalibur {

	class ImageParser {

	public:

		static unsigned char* Parse(const std::string& file, int* width, int* height, int* channels, int reqeryChannel) {
			auto data = stbi_load(file.c_str(), width, height, channels, reqeryChannel);
			return data;
		}
	};
}