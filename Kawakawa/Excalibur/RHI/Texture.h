#pragma once
#include <string>
#include "../Interface/IResource.h"

namespace Excalibur
{
	class ITexture : public IRenderResource
	{
	public:
		virtual void Initialize(const std::string& filepath) = 0;
		virtual void Finialize() = 0;
	};

}