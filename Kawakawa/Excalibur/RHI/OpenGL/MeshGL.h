#pragma once

#include "../MeshBase.h"
#include "../../Core/Object/World.h"
#include <glad/glad.h>

namespace Excalibur
{
	class MeshGL : public IMesh
	{

	public:
		MeshGL(aiMesh* mesh, const aiScene* world);
		MeshGL(void* data, int count, VertexFormat vf);
		virtual ~MeshGL();
		virtual void Initialize(aiMesh* mesh);
		virtual void Initialize(void* data, int count, VertexFormat vf);
		virtual void Render(World* world, const Matrix4f& worldMatrix);
		virtual void Finialize();

	private:
		GLenum GetMode();

		GLuint m_VAO;
	};
}