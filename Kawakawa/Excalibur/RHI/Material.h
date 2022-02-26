#pragma once

#include "../Interface/IResource.h"
#include "Shader.h"
#include "Texture.h"
#include "SamplerState.h"

#include <unordered_map>

namespace Excalibur 
{
	
	class IMaterial : IRenderResource
	{
	public:

		virtual void Initialize() = 0;
		virtual void Finialize() = 0;

		virtual void Apply(ConstantBuffer cb) = 0;

		virtual	void SetShader(std::shared_ptr<IShader> shader) { m_Shader = shader; }
		virtual std::shared_ptr<IShader> GetShader() { return m_Shader; }

		virtual void SetShaderParamter(std::string name, Vector4f value) { m_Parameters[name] = value; }
		virtual void SetTexture(std::string name, std::shared_ptr<ITexture> tex) { m_Textures[name] = tex; }
		virtual void SerSamplerState(std::string name, std::shared_ptr<ISamplerState> sampleState) { m_SamplerState[name] = sampleState; }

	protected:
		std::shared_ptr<IShader> m_Shader;
		std::unordered_map<std::string, Vector4f> m_Parameters;
		std::unordered_map<std::string, std::shared_ptr<ITexture>> m_Textures;
		std::unordered_map<std::string, std::shared_ptr<ISamplerState>> m_SamplerState;
	};

}