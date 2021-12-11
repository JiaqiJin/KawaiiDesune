#pragma once

#include "../Interface/IResourceh.h"
#include "ShaderBase.h"

namespace Excalibur
{
	class MaterialBase : public IResouceBase
	{
	public:
		virtual void Initialize() = 0;
		virtual void Finalize() = 0;
		virtual void Apply(ConstantBuffer cb) = 0;
		virtual void SetShaderParamter(std::string name, Vector4f value) { m_Parameters[name] = value; };
		virtual std::shared_ptr<ShaderBase> GetShader() { return m_Shader; }
		virtual void SetShader(std::shared_ptr<ShaderBase> shader) { m_Shader = shader; }
	protected:
		std::shared_ptr<ShaderBase> m_Shader;
		std::unordered_map<std::string, Vector4f> m_Parameters;
	};
}