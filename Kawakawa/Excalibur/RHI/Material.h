#pragma once

#include "../Interface/IResource.h"
#include "Shader.h"
#include "Texture.h"
#include "SamplerState.h"

#include <unordered_map>

namespace Excalibur 
{
	
	class Material : IRenderResource
	{
	public:
		virtual void Initialize();
		virtual void Finialize();
		virtual	void SetShader(std::shared_ptr<Shader>);
		virtual std::shared_ptr<Shader>	GetShader();
		virtual void Apply(ConstantBuffer cb);
		virtual void SetShaderParamter(std::string name, Vector4f value);
		virtual void SetTexture(std::string name, std::shared_ptr<Texture> tex);
		virtual void SerSamplerState(std::string name, std::shared_ptr<SamplerState>);

	protected:
		std::shared_ptr<Shader>	m_Shader;
		std::unordered_map<std::string, Vector4f> m_Parameters;
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
		std::unordered_map<std::string, std::shared_ptr<SamplerState>> m_SamplerState;
	};

}