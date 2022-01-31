#pragma once

#include "../GraphicsMgr.h"
#include "../../Core/Math/KawaiiMath.h"

#include <memory>

namespace Excalibur
{
	class GraphicsMgrGL : public GraphicsManager
	{
	public:
		virtual	int	Initialize() noexcept;
		virtual	void Finalize() noexcept ;
		virtual	void Tick() noexcept;
		virtual	void Present();
		virtual	void ClearRenderTarget(float r, float g, float b, float a);

		virtual	std::shared_ptr<IVertexBuffer> CreateVertexBuffer(void* data, int count, VertexFormat vf);

		virtual	std::shared_ptr<IIndexBuffer> CreateIndexBuffer(void* data, int count, IndexFormat iformat);

		virtual	std::shared_ptr<IMesh> CreateRenderMesh(aiMesh* mesh, const aiScene* world);
		virtual	std::shared_ptr<IMesh> CreateRenderMeshDebug(void* data, int count, VertexFormat vf);

		virtual	std::shared_ptr<ITexture> CreateTexture2D(const std::string& path);
		virtual	std::shared_ptr<SamplerState> CreateSamplerState();

		virtual	void LoadShaders();
		virtual	void UseShader(std::shared_ptr<IShader>);
		virtual	std::shared_ptr<IShader> GetShader(const std::string& shaderName);

		virtual	void Draw(unsigned int vcount, unsigned int start);
		virtual	void DrawIndexed(unsigned int icount, unsigned int start, int baseLoc);
	};
}