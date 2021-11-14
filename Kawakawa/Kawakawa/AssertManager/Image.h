#pragma once

#include <iostream>
#include <stdint.h>
namespace Kawaii::Asset
{
	class Image
	{
	public:
		Image(uint32_t width, uint32_t height, uint32_t bitcount, uint32_t pitch, size_t dataSize);
		Image() = default;

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }
		inline uint32_t GetBitcount() const { return m_Bitcount; }
		inline uint32_t GetPitch() const { return m_Pitch; }
		inline size_t GetDataSize() const { return m_DataSize; }

		friend std::ostream& operator<<(std::ostream& out, const Image& image);

	private:
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		uint32_t m_Bitcount = 0;
		uint32_t m_Pitch = 0;
		size_t m_DataSize = 0;
	};
}