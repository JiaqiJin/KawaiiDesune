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
	public:
		virtual	bool InitializeFromFile(const std::string& vsPath, const std::string& psPath);
		virtual	void Use();
		virtual void SetConstantBuffer(const ConstantBuffer& cbuffer);
	public:
		GLuint mProgram;
	};
}