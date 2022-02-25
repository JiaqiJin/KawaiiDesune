#include "Material.h"

namespace Excalibur
{
	void Material::Initialize()
	{
	}

	void Material::Finialize()
	{
	}

	void Material::SetShader(std::shared_ptr<Shader> shader)
	{
		m_Shader = shader;
	}

	std::shared_ptr<Shader> Material::GetShader()
	{
		return m_Shader;
	}

	void Material::Apply(ConstantBuffer cb)
	{
	}

	void Material::SetShaderParamter(std::string name, Vector4f value)
	{
		m_Parameters[name] = value;
	}

	void Material::SetTexture(std::string name, std::shared_ptr<Texture> tex)
	{
		m_Textures[name] = tex;
	}

	void Material::SerSamplerState(std::string name, std::shared_ptr<SamplerState> sampler)
	{
		m_SamplerState[name] = sampler;
	}

}