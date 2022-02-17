#pragma once

#include "../Interface/IModule.h"
#include "VertexBufferBase.h"
#include "IndexBufferBase.h"
#include "TexturehBase.h"
#include "ShaderBase.h"
#include "MeshBase.h"
#include "SamplerState.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

namespace Excalibur
{
	class GraphicsManager : public ITickableModule 
	{
	public:
		virtual	int	Initialize() noexcept = 0;
		virtual	void Finalize() noexcept = 0;

		virtual	void Present() = 0;
		virtual	void ClearRenderTarget(float r, float g, float b, float a) = 0;

		virtual	std::shared_ptr<IVertexBuffer> CreateVertexBuffer(void* data, int count, VertexFormat vf) = 0;

		virtual	std::shared_ptr<IIndexBuffer> CreateIndexBuffer(void* data, int count, IndexFormat iformat) = 0;

		virtual	std::shared_ptr<IMesh> CreateRenderMesh(aiMesh* mesh, const aiScene* world) = 0;
		virtual	std::shared_ptr<IMesh> CreateRenderMeshDebug(void* data, int count, VertexFormat vf) = 0;

		virtual	std::shared_ptr<ITexture> CreateTexture2D(const std::string& path) = 0;
		virtual	std::shared_ptr<SamplerState> CreateSamplerState() = 0;

		virtual	void LoadShaders() = 0;
		virtual	void UseShader(std::shared_ptr<IShader>) = 0;
		virtual	std::shared_ptr<IShader> GetShader(const std::string& shaderName) = 0;

		virtual	void Draw(unsigned int vcount, unsigned int start) = 0;
		virtual	void DrawIndexed(unsigned int icount, unsigned int start, int baseLoc) = 0;

	protected:
		std::unordered_map<std::string, std::shared_ptr<IShader>> m_Shaders;
		std::unordered_map<std::string, std::shared_ptr<ITexture>> m_Textures;
	};
}