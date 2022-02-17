#include "ImageParser.h"
#include "stb_image.h"

namespace Excalibur
{
	unsigned char* ImageParser::Parse(const std::string& file, int* width, int* height, int* channels, int reqeryChannel)
	{
		auto data = stbi_load(file.c_str(), width, height, channels, reqeryChannel);
		return data;
	}

	void ImageParser::Free(void* p)
	{
		STBI_FREE(p);
	}
}