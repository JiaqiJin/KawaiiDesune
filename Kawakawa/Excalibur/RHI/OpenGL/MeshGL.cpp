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
	MeshGL::MeshGL(aiMesh* mesh, const aiScene* world)
	{
		auto mgrgl = (GraphicsMgrGL*)GApp->mGraphicsManager;
		Initialize(mesh);

		auto material = world->mMaterials[mesh->mMaterialIndex];
		aiString name;
		aiGetMaterialString(material, AI_MATKEY_NAME, &name);
		m_Material = std::make_shared<MaterialGL>();
		aiColor4D diffuse;
		aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
		auto shader = mgrgl->GetShader("pbr");
		m_Material->SetShader(shader);
		m_Material->SetShaderParamter("color", Vector4f(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
	}

	MeshGL::MeshGL(void* data, int count, VertexFormat vf)
	{
		auto mgrgl = (GraphicsMgrGL*)GApp->mGraphicsManager;
		Initialize(data, count, vf);
		m_Material = std::make_shared<MaterialGL>();
		auto shader = mgrgl->GetShader("debug");
		m_Material->SetShader(shader);
	}

	MeshGL::~MeshGL()
	{
		Finialize();
	}

	void MeshGL::Initialize(aiMesh* mesh)
	{
		if (!mesh)
			return;

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		auto count = mesh->mNumVertices;
		// Position
		if (mesh->HasPositions()) 
		{
			m_Positions = std::make_shared<VertexBufferGL>(mesh->mVertices, count, VertexFormat::VF_P3F, 0);
		}

		// Normal
		if (mesh->HasNormals())
		{
			m_Normals = std::make_shared<VertexBufferGL>(mesh->mNormals, count, VertexFormat::VF_N3F, 1);
		}

		// Texture
		if (mesh->HasTextureCoords(0))
		{
			float* texCoords = (float*)malloc(sizeof(float) * 2 * mesh->mNumVertices);
			for (unsigned int k = 0; k < mesh->mNumVertices; k++) 
			{
				texCoords[k * 2] = mesh->mTextureCoords[0][k].x;
				texCoords[k * 2 + 1] = mesh->mTextureCoords[0][k].y;
			}
			m_TexCoords = std::make_shared<VertexBufferGL>(texCoords, count, VertexFormat::VF_T2F, 2);
			free(texCoords);
		}

		if (mesh->HasFaces())
		{
			unsigned int count = 3 * mesh->mNumFaces;
			unsigned int* idata = new unsigned int[count];
			for (int i = 0; i < mesh->mNumFaces; i++)
			{
				auto face = mesh->mFaces[i];
				idata[i * 3] = face.mIndices[0];
				idata[i * 3 + 1] = face.mIndices[1];
				idata[i * 3 + 2] = face.mIndices[2];
			}
			m_Indexes = std::make_shared<IndexBufferGL>(idata, count, IndexFormat::IF_UINT32);
			free(idata);
		}

		if (mesh->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_POINT) {
			m_Type = PrimitiveType::PT_POINT;
		}
		else if (mesh->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_LINE) {
			m_Type = PrimitiveType::PT_LINE;
		}
		else if (mesh->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_TRIANGLE) {
			m_Type = PrimitiveType::PT_TRIANGLE;
		}
		else {
			assert(false);
		}
	}

	void MeshGL::Initialize(void* data, int count, VertexFormat vf)
	{

	}

	void MeshGL::Render(World* world, const Matrix4f& worldMatrix)
	{

	}

	void MeshGL::Finialize()
	{
		m_Positions = nullptr;
		m_Normals = nullptr;
		m_TexCoords = nullptr;
		m_Indexes = nullptr;
		glDeleteVertexArrays(1, &m_VAO);
	}
}