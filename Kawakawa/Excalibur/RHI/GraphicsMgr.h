#pragma once

#include "../Interface/IModule.h"
#include "VertexBufferBase.h"
#include "IndexBufferBase.h"
#include "TexturehBase.h"
#include "ShaderBase.h"
#include "MeshBase.h"

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

		virtual std::shared_ptr<VertexBufferBase> CreateVertexBuffer(void* data, int count, VertexFormat vf) = 0;
		virtual std::shared_ptr<IndexBufferBase> CreateIndexBuffer(void* data, int count) = 0;
		virtual std::shared_ptr<TextureBase> CreateTexture2D(const std::string& path) = 0;

		virtual	std::shared_ptr<MeshBase> CreateRenderMesh(aiMesh* mesh, const aiScene* world) = 0;

		virtual void LoadShader() = 0;
		virtual void UseShader(std::shared_ptr<ShaderBase> shader) = 0;
		virtual std::shared_ptr<ShaderBase> GetShader(const std::string& name) = 0;

	protected:
		std::unordered_map<std::string, std::shared_ptr<ShaderBase>> m_Shaders;
	};
}