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
		LoadShaders();

		return 0;
	}


	void GraphicsMgrGL::Finalize() noexcept
	{

	}

	void GraphicsMgrGL::Tick() noexcept
	{
	}

	void GraphicsMgrGL::Present() noexcept
	{
		glFlush();
	}

	void GraphicsMgrGL::ClearRenderTarget(float r, float g, float b, float a) noexcept
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	std::shared_ptr<IVertexBuffer> GraphicsMgrGL::CreateVertexBuffer(void* data, int count, VertexFormat vf) noexcept
	{
		auto ptr = std::make_shared<VertexBufferGL>(data, count, vf, 0);
		return ptr;
	}

	std::shared_ptr<IIndexBuffer> GraphicsMgrGL::CreateIndexBuffer(void* data, int count, IndexFormat iformat) noexcept
	{
		return nullptr;
	}

	std::shared_ptr<IMesh> GraphicsMgrGL::CreateRenderMesh() noexcept
	{
		return std::shared_ptr<MeshGL>();
	}

	std::shared_ptr<IMesh> GraphicsMgrGL::CreateRenderMesh(aiMesh* mesh, const aiScene* world) noexcept
	{
		auto ptr = std::make_shared<MeshGL>(mesh, world);
		return ptr;
	}

	std::shared_ptr<IMesh> GraphicsMgrGL::CreateRenderMeshDebug(void* data, int count, VertexFormat vf) noexcept
	{
		auto ptr = std::make_shared<MeshGL>(data, count, vf);
		return ptr;
	}

	std::shared_ptr<IMesh> GraphicsMgrGL::CreateRenderMeshUI() noexcept
	{
		return std::shared_ptr<MeshGL>();
	}

	std::shared_ptr<ITexture> GraphicsMgrGL::CreateTexture2D(const std::string& path) noexcept
	{
		return std::shared_ptr<ITexture>();
	}

	std::shared_ptr<ITexture> GraphicsMgrGL::CreateTexture2D(int width, int height, unsigned char* data) noexcept
	{
		return std::shared_ptr<ITexture>();
	}

	std::shared_ptr<ITexture> GraphicsMgrGL::CreateTextureCubemap(const std::string& path) noexcept
	{
		return std::shared_ptr<ITexture>();
	}

	void GraphicsMgrGL::LoadShaders() noexcept
	{
		std::string pbrShaderVS = "Asset/Shaders/simple.vs";
		std::string pbrShaderPS = "Asset/Shaders/simple.fs";
		auto pbrShader = std::make_shared<ShaderGL>(pbrShaderVS, pbrShaderPS);
		mShaders["pbr"] = pbrShader;

		/*std::string debugShaderVS = "Asset/Shaders/gl_debug.vs";
		std::string debugShaderPS = "Asset/Shaders/gl_debug.ps";
		auto debugShader = std::make_shared<ShaderGL>(debugShaderVS, debugShaderPS);
		mShaders["debug"] = debugShader;*/
	}


	void GraphicsMgrGL::UseShader(std::shared_ptr<IShader> shader) noexcept
	{
		if (!shader) {
			assert(false);
		}
		shader->Use();
	}

	std::shared_ptr<IShader> GraphicsMgrGL::GetShader(const std::string& shaderName) noexcept
	{
		return mShaders[shaderName];
	}

	void GraphicsMgrGL::Draw(unsigned int vcount, unsigned int start) noexcept
	{
	}

	void GraphicsMgrGL::DrawIndexed(unsigned int icount, unsigned int start, int baseLoc) noexcept
	{
	}
}