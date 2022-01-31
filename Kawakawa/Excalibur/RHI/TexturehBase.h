#pragma once

#include "../Interface/IResourceh.h"

namespace Excalibur
{
	class ITexture : public IResouceBase
	{
	public:
		virtual void Initialize(const std::string& filepath) = 0;
		virtual void Finialize() = 0;

	};

}