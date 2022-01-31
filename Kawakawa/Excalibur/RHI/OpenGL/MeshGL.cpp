#include "MeshGL.h"
#include "GraphicsMgrGL.h"
#include "VertexBufferGL.h"
#include "IndexBufferGL.h"
#include "MaterialGL.h"
#include "../../Core/Object/World.h"
#include "../../Platform/Application.h"
#include "ShaderGL.h"
#include "../../Core/Math/EigenMath.h"

namespace Excalibur
{
	MeshGL::MeshGL()
	{
	}

	MeshGL::MeshGL(aiMesh* mesh, const aiScene* world)
	{
		
	}

	MeshGL::MeshGL(void* data, int count, VertexFormat vf)
	{
		
	}

	MeshGL::~MeshGL()
	{
		
	}


	void MeshGL::Initialize(aiMesh* mesh) noexcept
	{
		
	}

	void MeshGL::Initialize(void* data, int count, VertexFormat vf) noexcept
	{
		
	}

	void MeshGL::InitializeUI() noexcept
	{
	}

	void MeshGL::InitializeTerrain() noexcept
	{
	}

	void MeshGL::Render(Entity* self) noexcept
	{
		
	}

	GLenum MeshGL::GetMode()
	{
		return 1;
	}

	void MeshGL::Render(const Matrix4x4f& world, const Matrix4x4f& view, const Matrix4x4f& projection) noexcept
	{

	}
}