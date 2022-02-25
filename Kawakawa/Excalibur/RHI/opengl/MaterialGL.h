#pragma once
#include "../Material.h"

namespace Excalibur 
{
	class MaterialGL : public Material
	{
	public:
		virtual void Apply(ConstantBuffer cb);
	};
}
