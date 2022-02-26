#pragma once
#include "../Material.h"

namespace Excalibur 
{
	class MaterialGL : public IMaterial
	{
	public:
		virtual void Initialize() {}
		virtual void Finialize() {}

		virtual void Apply(ConstantBuffer cb);
	};
}
