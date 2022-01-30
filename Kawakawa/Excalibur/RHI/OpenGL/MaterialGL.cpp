#include "MaterialGL.h"
#include "ShaderGL.h"

namespace Excalibur
{
	void MaterialGL::Apply(ConstantBuffer cb) noexcept
	{
		mShader->Use();

		for (auto pair : mParameters) {
			if (pair.first == "color") {
				cb.debugColor = pair.second;
			}
		}
		mShader->SetConstantBuffer(cb);
	}

	void MaterialGL::ApplyLight(ConstantBufferLighting cb) noexcept
	{
	}
}