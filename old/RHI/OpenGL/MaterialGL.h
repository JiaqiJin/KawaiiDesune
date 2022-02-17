#pragma once

#include "../MaterialBase.h"

namespace Excalibur
{
	class MaterialGL : public IMaterial
	{
	public:
		virtual void Apply(ConstantBuffer cb);
	};
}