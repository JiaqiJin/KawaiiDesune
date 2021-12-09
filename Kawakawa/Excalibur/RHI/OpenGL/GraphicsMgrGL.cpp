#include "GraphicsMgrGL.h"
#include "VertexBufferGL.h"
#include "IndexBufferGL.h"
#include "ShaderGL.h"
#include "MeshGL.h"

#include <iostream>
#include <glad/glad.h>

namespace Excalibur
{
	int GraphicsMgrGL::Initialize() noexcept
	{
		int result;
		result = gladLoadGL();
		assert(result);

		if (GLAD_GL_VERSION_3_0) {
			// Enable depth testing.
			glClearDepth(1.0f);
			// Enable depth testing.
			glEnable(GL_DEPTH_TEST);
		}
		glViewport(0, 0, 1024, 768);
		std::cout << "OpenGl Version: " << GLVersion.major << "." << GLVersion.minor << " loaded";
		LoadShader();

		return 0;
	}

	void GraphicsMgrGL::Finalize() noexcept
	{

	}

	void GraphicsMgrGL::Tick() noexcept
	{
	}

	void GraphicsMgrGL::Present()
	{
		glFlush();
	}

	void GraphicsMgrGL::ClearRenderTarget(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	std::shared_ptr<VertexBufferBase> GraphicsMgrGL::CreateVertexBuffer(void* data, int count, VertexFormat vf)
	{
		auto ret = std::make_shared<VertexBufferGL>(data, count, vf, 0);
		return ret;
	}

	std::shared_ptr<IndexBufferBase> GraphicsMgrGL::CreateIndexBuffer(void* data, int count)
	{
		auto ret = std::make_shared<IndexBufferGL>(data, count);
		return ret;
	}

	std::shared_ptr<TextureBase> GraphicsMgrGL::CreateTexture2D(const std::string& path)
	{
		return nullptr;
	}

	std::shared_ptr<MeshBase> GraphicsMgrGL::CreateRenderMesh(aiMesh* mesh, const aiScene* world)
	{
		auto ret = std::make_shared<MeshGL>(mesh, world);
		return ret;
	}

	void GraphicsMgrGL::UseShader(std::shared_ptr<ShaderBase> shader)
	{
		if (!shader)
			assert(false);

		shader->Use();
	}

	void GraphicsMgrGL::LoadShader()
	{
		std::string simpleShaderVS = "Asset/Shaders/simple.vs";
		std::string simpleShaderFS = "Asset/Shaders/simple.fs";
		auto simpleShader = std::make_shared<ShaderGL>(simpleShaderVS, simpleShaderFS);
		m_Shaders["simple"] = simpleShader;
	}

	std::shared_ptr<ShaderBase> GraphicsMgrGL::GetShader(const std::string& name)
	{
		return m_Shaders[name];
	}
}