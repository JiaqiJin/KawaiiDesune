#pragma once

#include "../Interface/IResourceh.h"

namespace Excalibur
{
	enum TextureType {
		Default = 1,
		Cubemap = 2,
	};

	class ITexture : public IResouceBase
	{
	public:
		ITexture(TextureType type) : mType(type) {}
		virtual void	InitializeDefault(const std::string& filepath) noexcept = 0;
		virtual void	InitializeCubemap(const std::string& filepath) noexcept = 0;
	protected:
		TextureType mType;
	};

}