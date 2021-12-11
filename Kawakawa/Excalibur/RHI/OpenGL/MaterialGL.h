#pragma once

#include "../MaterialBase.h"

namespace Excalibur
{
	class MaterialGL : public MaterialBase
	{
	public:
		virtual void Initialize();
		virtual void Finalize();
		virtual void Apply(ConstantBuffer cb);
	};
}