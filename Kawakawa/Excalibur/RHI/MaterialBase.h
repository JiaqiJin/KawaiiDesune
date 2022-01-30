#pragma once

#include "../Interface/IResourceh.h"
#include "ShaderBase.h"
#include "TexturehBase.h"

#include <unordered_map>

namespace Excalibur
{
	class IMaterial : public IResouceBase
	{
	public:
		virtual void		Apply(ConstantBuffer cb) noexcept = 0;
		virtual void		ApplyLight(ConstantBufferLighting cb) noexcept = 0;

		void SetName(std::string name) { mName = name; }
		std::string GetName() { return mName; }

		void SetShaderParamter(std::string name, Vector4f value) { mParameters[name] = value; }
		Vector4f getShaderParamter(std::string name) { return mParameters[name]; }

		void SetTexture(std::string name, std::shared_ptr<ITexture> tex) { mTextures[name] = tex; }
		std::shared_ptr<ITexture> getTexture(std::string name) { return mTextures[name]; }

		void SetShader(std::shared_ptr<IShader> shader) { mShader = shader; }
		std::shared_ptr<IShader>		GetShader() { return mShader; }

	protected:
		std::string															mName;
		std::shared_ptr<IShader>											mShader;
		std::unordered_map<std::string, std::shared_ptr<ITexture>>		mTextures;
		std::unordered_map<std::string, Vector4f>						mParameters;
	};
}