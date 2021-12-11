#include "MaterialGL.h"
#include "ShaderGL.h"

namespace Excalibur
{
	void MaterialGL::Initialize()
	{

	}

	void MaterialGL::Finalize()
	{

	}

	void MaterialGL::Apply(ConstantBuffer cb)
	{
		m_Shader->Use();
		for (auto pair : m_Parameters) {
			if (pair.first == "color") {
				cb.debugColor = pair.second;
			}
		}

		m_Shader->SetConstantBuffer(cb);
	}
}