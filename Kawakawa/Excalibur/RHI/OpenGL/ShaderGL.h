#pragma once

#include "../ShaderBase.h"
#include <glad/glad.h>

namespace Excalibur
{
	class ShaderGL : public ShaderBase
	{
	public:
		ShaderGL(const std::string& vsPath, const std::string& psPath);
		virtual ~ShaderGL();
		virtual bool Initialize(const std::string& VS_Path, const std::string& PS_Path);
		virtual void Use();

	protected:
		GLuint mProgram;
	};
}