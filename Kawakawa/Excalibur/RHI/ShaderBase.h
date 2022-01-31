#pragma once

#include "../Interface/IResourceh.h"
#include <string>

namespace Excalibur
{
	class IShader : public IResouceBase 
	{
	public:
		virtual	bool InitializeFromFile(const std::string& vsPath, const std::string& psPath) = 0;
		virtual void Use() = 0;
		virtual void Finialize() = 0;
		virtual void SetConstantBuffer(const ConstantBuffer& cbuffer) = 0;
	};
}