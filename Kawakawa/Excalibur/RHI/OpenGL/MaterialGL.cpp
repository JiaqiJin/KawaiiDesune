#include "MaterialGL.h"
#include "ShaderGL.h"

namespace Excalibur
{
	void MaterialGL::Apply(ConstantBuffer cb)
	{
		mShader->Use();

		for (auto pair : mParameters)
		{
			if (pair.first == "color") 
				cb.debugColor = pair.second;
		}
		mShader->SetConstantBuffer(cb);
	}
}