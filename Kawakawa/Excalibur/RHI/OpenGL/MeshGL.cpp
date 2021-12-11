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
		auto mgrgl = (GraphicsMgrGL*)GApp->mGraphicsManager;
		Initialize(mesh);

		auto material = world->mMaterials[mesh->mMaterialIndex];
		aiString name;
		aiGetMaterialString(material, AI_MATKEY_NAME, &name);
		m_Material = std::make_shared<MaterialGL>();
		aiColor4D diffuse;
		aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
		auto shader = mgrgl->GetShader("simple");
		m_Material->SetShader(shader);
		m_Material->SetShaderParamter("color", Vector4f(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
	}

	MeshGL::MeshGL(void* data, int count, VertexFormat vf)
	{
		auto mgr = (GraphicsMgrGL*)GApp->mGraphicsManager;
		Initialize(data, count, vf);

		auto shader = mgr->GetShader("debug");

	}

	MeshGL::~MeshGL()
	{
		m_Position = nullptr;
		m_Normal = nullptr;
		m_Indexes = nullptr;
		glDeleteVertexArrays(1, &mVAO);
	}

	void MeshGL::Initialize(aiMesh* mesh)
	{
		if (!mesh) {
			return;
		}

		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		auto count = mesh->mNumVertices;
		if (mesh->HasPositions()) {
			m_Position = std::make_shared<VertexBufferGL>(mesh->mVertices, count, VertexFormat::VF_3F, 0);
		}

		if (mesh->HasNormals()) {
			m_Normal = std::make_shared<VertexBufferGL>(mesh->mNormals, count, VertexFormat::VF_3F, 1);
		}

		if (mesh->HasTextureCoords(0)) 
		{
			float* texCoords = (float*)malloc(sizeof(float) * 2 * mesh->mNumVertices);
			for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
				texCoords[k * 2] = mesh->mTextureCoords[0][k].x;
				texCoords[k * 2 + 1] = mesh->mTextureCoords[0][k].y;
			}
			m_TexCoord = std::make_shared<VertexBufferGL>(texCoords, count, VertexFormat::VF_2F, 2);
			free(texCoords);
		}

		if (mesh->HasFaces()) {
			unsigned int count = 3 * mesh->mNumFaces;
			unsigned int* idata = new unsigned int[count];
			for (int i = 0; i < mesh->mNumFaces; ++i) {
				auto face = mesh->mFaces[i];
				idata[i * 3] = face.mIndices[0];
				idata[i * 3 + 1] = face.mIndices[1];
				idata[i * 3 + 2] = face.mIndices[2];
			}
			m_Indexes = std::make_shared<IndexBufferGL>(idata, count);
			free(idata);
		}
	}

	void MeshGL::Initialize(void* data, int count, VertexFormat vf)
	{
		auto mgrgl = (GraphicsMgrGL*)GApp->mGraphicsManager;
		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		m_Position = mgrgl->CreateVertexBuffer(data, count, vf);
	}

	void MeshGL::Render(World* world, const Matrix4f& worldMatrix)
	{
		ConstantBuffer cb;
		auto camera = world->GetCameraSystem()->GetMainCamera()->GetComponent<CameraComponent>();
		cb.world = worldMatrix.transpose();
		cb.view = camera->GetViewMatrix().transpose();
		cb.projection = camera->GetPerspectiveMatrix().transpose();
		//m_Material->Apply(cb);

		glBindVertexArray(mVAO);
		if (m_Indexes)
		{
			glDrawElements(GL_TRIANGLES, m_Indexes->GetIndexCount(), GL_UNSIGNED_INT, 0x00);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0x00, m_Position->GetVertextCount());
		}
	}
}