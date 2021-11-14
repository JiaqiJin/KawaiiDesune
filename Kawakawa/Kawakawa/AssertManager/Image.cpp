#include "Image.h"

#include <string>
#include <array>

#include "../Core/MemoryManager.h"

namespace Kawaii::Asset
{
    Image::Image(uint32_t width, uint32_t height, uint32_t bitcount, uint32_t pitch, size_t dataSize)
        : m_Width(width),
        m_Height(height),
        m_Bitcount(bitcount),
        m_Pitch(pitch),
        Core::Buffer(dataSize)
    {

    }

    std::ostream& operator<<(std::ostream& out, const Image& image)
    {
        out << "Image" << std::endl;
        out << "-----" << std::endl;
        out << "Width: " << image.m_Width << std::endl;
        out << "Height: " << image.m_Height << std::endl;
        out << "Bit Count: " << image.m_Bitcount << std::endl;
        out << "Pitch: " << image.m_Pitch << std::endl;
        out << "Data Size: " << image.GetDataSize() << std::endl;

        return out;
    }
}