#pragma once

#include "../Interface/IResource.h"

using namespace std;

namespace Excalibur
{
	class IShader : public IRenderResource
	{
	public:
		virtual	bool InitializeFromFile(const string& vsPath, const string& psPath) = 0;
		virtual void Use() = 0;
		virtual void Finialize() = 0;
		virtual void SetConstantBuffer(const ConstantBuffer& cbuffer) = 0;
	};

}