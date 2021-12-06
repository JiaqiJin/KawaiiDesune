#pragma once

#include "../Interface/IResourceh.h"

namespace Excalibur
{
	class TextureBase : public IResouceBase
	{
	public:
		TextureBase() {}
		virtual void Initialize() = 0;
	protected:
		
	};
}