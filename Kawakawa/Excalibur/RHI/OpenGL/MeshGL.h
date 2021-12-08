#pragma once

#include "../MeshBase.h"
#include <glad/glad.h>

namespace Excalibur
{
	class MeshGL : public MeshBase
	{
	public:
		MeshGL();
		MeshGL(aiMesh* mesh, const aiScene* world);
		MeshGL(void* data, int count, VertexFormat vf);
		virtual ~MeshGL();

		virtual void Initialize(aiMesh* mesh);
		virtual void Initialize(void* data, int count, VertexFormat vf);

		virtual void Render(const Matrix4x4f& worldMatrix);
	private:
		GLuint mVAO;
	};
}