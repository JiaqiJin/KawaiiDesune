#pragma once

#include "../Interface/IResourceh.h"
#include "VertexBufferBase.h"
#include "IndexBufferBase.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>     
#include <assimp/postprocess.h>

#include <memory>

namespace Excalibur
{
	class MeshBase : public IResouceBase
	{
	public:

	protected:
		std::shared_ptr<VertexBufferBase> m_Position;
		std::shared_ptr<VertexBufferBase> m_Normal;
		std::shared_ptr<IndexBufferBase> m_Indexes;
	};
}