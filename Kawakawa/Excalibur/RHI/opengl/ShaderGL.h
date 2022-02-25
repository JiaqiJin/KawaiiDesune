#pragma once

#include "../Shader.h"
#include "../../Vendor/Glad/include/glad/glad_wgl.h"

using namespace std;

namespace Excalibur 
{
	class ShaderGL : public Shader
	{
	public:
		ShaderGL(const string & vsPath, const string & psPath);
		virtual ~ShaderGL();
	public:
		virtual	bool InitializeFromFile(const string& vsPath, const string& psPath);
		virtual	void Use();
		virtual void Finialize();
		virtual void SetConstantBuffer(const ConstantBuffer& cbuffer);

	public:
		GLuint mProgram;
	};

}