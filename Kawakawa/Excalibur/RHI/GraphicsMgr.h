#pragma once

#include "../Interface/IModule.h"
#include "VertexBufferBase.h"
#include "IndexBufferBase.h"
#include "TexturehBase.h"
#include "ShaderBase.h"

namespace Excalibur
{
	class GraphicsManager : public ITickableModule
	{
	public:
		virtual	int	Initialize() noexcept = 0;
		virtual	void Finalize() noexcept = 0;

		/*virtual std::shared_ptr<VertexBufferBase> CreateVertexBuffer() = 0;
		virtual std::shared_ptr<IndexBufferBase> CreateIndexBuffer() = 0;
		virtual std::shared_ptr<TextureBase> CreateTexture2D() = 0;

		virtual void LoadShader() = 0;*/

	protected:

	};
}