#pragma once

#include "../Interface/IResourceh.h"
#include "ShaderBase.h"
#include "TexturehBase.h"
#include "SamplerState.h"

#include <memory>
#include <unordered_map>

namespace Excalibur
{
	class IMaterial : public IResouceBase
	{
	public:
		virtual void Initialize() = 0;
		virtual void Finialize() = 0;
	
		virtual void Apply(ConstantBuffer cb) = 0;

		virtual std::shared_ptr<IShader> GetShader() { return mShader; }
		virtual	void SetShader(std::shared_ptr<IShader> shader) { mShader = shader; }
		virtual void SetShaderParamter(std::string name, Vector4f value) { mParameters[name] = value; }
		virtual void SetTexture(std::string name, std::shared_ptr<ITexture> tex) { mTextures[name] = tex; }
		virtual void SerSamplerState(std::string name, std::shared_ptr<SamplerState> sampler) { mSamplerState[name] = sampler; }

	protected:
		std::shared_ptr<IShader> mShader;
		std::unordered_map<std::string, Vector4f> mParameters;
		std::unordered_map<std::string, std::shared_ptr<ITexture>> mTextures;
		std::unordered_map<std::string, std::shared_ptr<SamplerState>> mSamplerState;
	};
}