#pragma once
#include <string>
#include "../Interface/IResource.h"

namespace Excalibur
{
	class Texture : public IRenderResource
	{
	public:
		virtual void Initialize(const std::string& filepath);
		virtual void Finialize();
	};

}