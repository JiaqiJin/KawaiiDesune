#pragma once

#include "../Interface/IResourceh.h"
#include <string>

namespace Excalibur
{
	class ShaderBase : public IResouceBase
	{
	public:
		virtual bool Initialize(const std::string& VS_Path, const std::string& PS_Path) = 0;
		virtual void Use() = 0;
	};
}