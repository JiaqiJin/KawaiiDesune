#pragma once

#include "../Interface/IModule.h"
#include "../Interface/IResource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "RenderMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>      

#include <memory>
#include <unordered_map>

namespace Excalibur
{
	class GraphicsManager : public ITickableModule
	{
	public:
		virtual	int	Initialize() = 0;
		virtual	void Finalize() = 0;

		virtual	void Present() = 0;
		virtual	void ClearRenderTarget(float r, float g, float b, float a) = 0;

		virtual	std::shared_ptr<VertexBuffer>CreateVertexBuffer(void* data, int count, VertexFormat vf) = 0;

		virtual	std::shared_ptr<IndexBuffer>CreateIndexBuffer(void* data, int count, IndexFormat iformat) = 0;

		virtual	std::shared_ptr<RenderMesh>	CreateRenderMesh(aiMesh* mesh, const aiScene* world) = 0;
		virtual	std::shared_ptr<RenderMesh>	CreateRenderMeshDebug(void* data, int count, VertexFormat vf) = 0;

		virtual	std::shared_ptr<Texture> CreateTexture2D(const std::string& path) = 0;
		virtual	std::shared_ptr<SamplerState> CreateSamplerState() = 0;

		virtual	void LoadShaders() = 0;
		virtual	void UseShader(std::shared_ptr<Shader>) = 0;
		virtual	std::shared_ptr<Shader> GetShader(const std::string& shaderName) = 0;

		virtual	void Draw(unsigned int vcount, unsigned int start) = 0;
		virtual	void DrawIndexed(unsigned int icount, unsigned int start, int baseLoc) = 0;

	protected:
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
		std::unordered_map<std::string, std::shared_ptr<Texture>> mTextures;
	};

}