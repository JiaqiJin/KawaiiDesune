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
		virtual ~MeshGL();

		virtual void Initialize(aiMesh* mesh);

		virtual void Render();
	private:
		GLuint mVAO;
	};
}