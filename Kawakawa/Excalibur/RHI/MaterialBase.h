#pragma once

#include "../Interface/IResourceh.h"
#include "ShaderBase.h"
#include "TexturehBase.h"
#include "SamplerState.h"
#include "../Core/Math/EigenMath.h"

#include <memory>
#include <unordered_map>

namespace Excalibur
{
	class IMaterial : public IResouceBase
	{
	public:
		virtual void Initialize() {}
		virtual void Finialize() {}
		virtual void Apply(ConstantBuffer cb) = 0;

		virtual std::shared_ptr<IShader> GetShader() { return m_Shader; }
		virtual	void SetShader(std::shared_ptr<IShader> shader) { m_Shader = shader; }
		virtual void SetShaderParamter(std::string name, Vector4f value) { m_Parameters[name] = value; }
		virtual void SetTexture(std::string name, std::shared_ptr<ITexture> tex) { m_Textures[name] = tex; }
		virtual void SetSamplerState(std::string name, std::shared_ptr<SamplerState> sampler) { m_SamplerState[name] = sampler; }

	protected:
		std::shared_ptr<IShader> m_Shader;
		std::unordered_map<std::string, Vector4f> m_Parameters;
		std::unordered_map<std::string, std::shared_ptr<ITexture>> m_Textures;
		std::unordered_map<std::string, std::shared_ptr<SamplerState>> m_SamplerState;
	};
}