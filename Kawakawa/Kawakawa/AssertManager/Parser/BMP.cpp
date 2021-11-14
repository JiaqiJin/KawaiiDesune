#include "BMP.h"

#include <spdlog/spdlog.h>

#include "../../Math/KawaiiMath.h"

namespace Kawaii::Asset
{
    Image BmpParser::Parse(const Core::Buffer& buf)
    {
        Image img;
        const auto* pFileHeader =
            reinterpret_cast<const BITMAP_FILEHEADER*>(buf.GetData());
        const auto* pBmpHeader = reinterpret_cast<const BITMAP_HEADER*>(
            reinterpret_cast<const uint8_t*>(buf.GetData()) +
            BITMAP_FILEHEADER_SIZE);
        if (pFileHeader->Signature == 0x4D42 /* 'B''M' */) {
            std::cerr << "Asset is Windows BMP file" << std::endl;
            std::cerr << "BMP Header" << std::endl;
            std::cerr << "----------------------------" << std::endl;
            std::cerr << "File Size: " << pFileHeader->Size << std::endl;
            std::cerr << "Data Offset: " << pFileHeader->BitsOffset
                << std::endl;
            std::cerr << "Image Width: " << pBmpHeader->Width << std::endl;
            std::cerr << "Image Height: " << pBmpHeader->Height << std::endl;
            std::cerr << "Image Planes: " << pBmpHeader->Planes << std::endl;
            std::cerr << "Image BitCount: " << pBmpHeader->BitCount
                << std::endl;
            std::cerr << "Image Compression: " << pBmpHeader->Compression
                << std::endl;
            std::cerr << "Image Size: " << pBmpHeader->SizeImage << std::endl;

            auto width = pBmpHeader->Width;
            auto height = pBmpHeader->Height;
            auto bitcount = 32;
            auto byte_count = bitcount >> 3;
            auto pitch = ((width * byte_count) + 3) & ~3;
            auto data_size = (size_t)pitch * height;
            Image img(width, height, bitcount, pitch, data_size);
            auto data = reinterpret_cast<R8G8B8A8Unorm*>(img.GetData());

            if (bitcount < 24) 
            {
                std::cerr << "Sorry, only true color BMP is supported at now."
                    << std::endl;
            }
            else {
                const uint8_t* pSourceData =
                    reinterpret_cast<const uint8_t*>(buf.GetData()) +
                    pFileHeader->BitsOffset;
                for (int32_t y = height - 1; y >= 0; y--) {
                    for (uint32_t x = 0; x < width; x++) {
                        auto dst = reinterpret_cast<R8G8B8A8Unorm*>(
                            reinterpret_cast<uint8_t*>(data) +
                            (ptrdiff_t)pitch *
                            ((ptrdiff_t)height - y - 1) +
                            (ptrdiff_t)x * byte_count);
                        auto src = reinterpret_cast<const R8G8B8A8Unorm*>(
                            pSourceData + (ptrdiff_t)pitch * y +
                            (ptrdiff_t)x * byte_count);
                        dst->data[2] = src->data[0];
                        dst->data[1] = src->data[1];
                        dst->data[0] = src->data[2];
                        dst->data[3] = src->data[3];
                    }
                }
            }
        }
        return img;
    }
}