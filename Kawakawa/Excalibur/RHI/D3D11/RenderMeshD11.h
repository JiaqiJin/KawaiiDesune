#pragma once
#include "../MeshBase.h"

#include <d3d11.h>

namespace Excalibur
{
	class RenderMeshD11 : public MeshBase
	{
	public:
		RenderMeshD11(aiMesh* mesh, const aiScene* world);
		RenderMeshD11(void* data, int count, VertexFormat vf);
		virtual ~RenderMeshD11();
		virtual void		Initialize(aiMesh* mesh) noexcept;
		virtual void		Initialize(void* data, int count, VertexFormat vf) noexcept;
		virtual void		Render(World* world, const Matrix4f& worldMatrix) noexcept;
		virtual void		Finialize() noexcept;

	private:
		unsigned int*		stride;
		unsigned int*		offset;
		ID3D11Buffer **		vbuffers;
		unsigned int		vbcount;
	};

}