#pragma once

#include "../MeshBase.h"
#include "../../Core/Object/World.h"
#include <glad/glad.h>

namespace Excalibur
{
	class MeshGL : public IMesh
	{

	public:
		MeshGL();
		MeshGL(aiMesh* mesh, const aiScene* world);
		MeshGL(void* data, int count, VertexFormat vf);
		virtual ~MeshGL();
		virtual void		Initialize(aiMesh* mesh) noexcept;
		virtual void		Initialize(void* data, int count, VertexFormat vf) noexcept;
		virtual void		InitializeUI() noexcept;
		virtual void		InitializeTerrain() noexcept;

		virtual void		Render(Entity* self) noexcept;
		virtual void		Render(const Matrix4x4f& world, const Matrix4x4f& view, const Matrix4x4f& projection) noexcept;
	private:
		GLenum GetMode();

		GLuint mVAO;

	};
}