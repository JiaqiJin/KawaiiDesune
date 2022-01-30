#pragma once

#include "../ShaderBase.h"
#include <glad/glad.h>

namespace Excalibur
{
	class ShaderGL : public IShader
	{
	public:
		ShaderGL(const std::string& vsPath, const std::string& psPath);
		virtual ~ShaderGL();
	public:
		virtual	bool	InitializeFromFile(const std::string& vsPath, const std::string& psPath) noexcept;
		virtual	void	Use() noexcept;
		virtual void	SetConstantBuffer(const ConstantBuffer& cbuffer) noexcept;
		virtual void	SetConstantBufferLight(const ConstantBufferLighting& cbuffer) noexcept;
	public:
		GLuint mProgram;
	};
}