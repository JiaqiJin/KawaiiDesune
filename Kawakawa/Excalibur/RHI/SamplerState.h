#pragma once

#include "../Interface/IResource.h"

namespace Excalibur 
{
	class ISamplerState : public IRenderResource
	{
	public:
		virtual void Initialize() = 0;
		virtual void Finialize() = 0;
	};
	
}