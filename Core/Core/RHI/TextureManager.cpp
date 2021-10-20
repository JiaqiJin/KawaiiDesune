#include "../Kawaiipch.h"
#include "TextureManager.h"

#include "GraphicsCore.h"

#include "../Utils/FilesUtil.h"

namespace RHI
{
	namespace 
	{
		enum class TextureFormat
		{
			eDDS,
			eBMP,
			eJPG,
			ePNG,
			eTIFF,
			eGIF,
			eICO,
			eTGA,
			eHDR,
			ePIC,
			eNotSupported
		};

		TextureFormat GetTextureFormat(const std::string& path)
		{
			std::string extension = Util::GetExtension(path);
			std::transform(std::begin(extension), std::end(extension), std::begin(extension), [](char c) {return std::tolower(c); });

			if (extension == ".dds")
				return TextureFormat::eDDS;
			else if (extension == ".bmp")
				return TextureFormat::eBMP;
			else if (extension == ".jpg" || extension == ".jpeg")
				return TextureFormat::eJPG;
			else if (extension == ".png")
				return TextureFormat::ePNG;
			else if (extension == ".tiff" || extension == ".tif")
				return TextureFormat::eTIFF;
			else if (extension == ".gif")
				return TextureFormat::eGIF;
			else if (extension == ".ico")
				return TextureFormat::eICO;
			else if (extension == ".tga")
				return TextureFormat::eTGA;
			else if (extension == ".hdr")
				return TextureFormat::eHDR;
			else if (extension == ".pic")
				return TextureFormat::ePIC;
			else
				return TextureFormat::eNotSupported;
		}
	}

	TextureManager::TextureManager(GraphicCore* gfx)
		: m_gfx(gfx)
	{

	}

	void TextureManager::LoadTexture(const std::wstring& name)
	{

	}

	void TextureManager::LoadCubemap(const std::wstring& name)
	{

	}
}