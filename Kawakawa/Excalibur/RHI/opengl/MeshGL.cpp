#include "MeshGL.h"
#include "VertexBufferGL.h"
#include "IndexBufferGL.h"
#include "MaterialGL.h"
#include "GraphicsMgrGL.h"
#include "../../Core/Application/Application.h"

namespace Excalibur
{
	MeshGL::MeshGL(aiMesh* mesh, const aiScene* world)
	{
		auto mgrgl = (GraphicsMgrGL*)GApp->m_GraphicsManager;
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
		auto mgrgl = (GraphicsMgrGL*)GApp->m_GraphicsManager;
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
		if (!mesh) {
			return;
		}

		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		auto count = mesh->mNumVertices;
		if (mesh->HasPositions()) {
			m_Positions = std::make_shared<VertexBufferGL>(mesh->mVertices, count, VertexFormat::VF_P3F, 0);
		}

		if (mesh->HasNormals()) {
			m_Normals = std::make_shared<VertexBufferGL>(mesh->mNormals, count, VertexFormat::VF_N3F, 1);
		}

		if (mesh->HasTextureCoords(0)) {
			float* texCoords = (float*)malloc(sizeof(float) * 2 * mesh->mNumVertices);
			for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
				texCoords[k * 2] = mesh->mTextureCoords[0][k].x;
				texCoords[k * 2 + 1] = mesh->mTextureCoords[0][k].y;
			}
			m_TexCoords = std::make_shared<VertexBufferGL>(texCoords, count, VertexFormat::VF_T2F, 2);
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
		auto mgrgl = (GraphicsMgrGL*)GApp->m_GraphicsManager;
		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		m_Positions = mgrgl->CreateVertexBuffer(data, count, vf);
		m_Type = PrimitiveType::PT_LINE;
	}

	void MeshGL::Render(World* world, const Matrix4f& worldMatrix)
	{
		ConstantBuffer cb;
		auto camera = world->GetCameraSystem()->GetMainCamera()->GetComponent<CameraComponent>();
		cb.world = worldMatrix.transpose();
		cb.view = camera->GetViewMatrix().transpose();
		cb.projection = camera->GetPerspectiveMatrix().transpose();
		m_Material->Apply(cb);

		glBindVertexArray(mVAO);
		if (m_Indexes) {
			glDrawElements(GetMode(), m_Indexes->GetIndexCount(), GL_UNSIGNED_INT, 0x00);
		}
		else {
			glDrawArrays(GetMode(), 0x00, m_Positions->GetVertexCount());
		}
	}

	void MeshGL::Finialize()
	{
		m_Positions = nullptr;
		m_Normals = nullptr;
		m_TexCoords = nullptr;
		m_Indexes = nullptr;
		glDeleteVertexArrays(1, &mVAO);
	}

	GLenum MeshGL::GetMode()
	{
		switch (m_Type)
		{
		case PT_POINT:
			return GL_POINTS;
		case PT_LINE:
			return GL_LINES;
		case PT_TRIANGLE:
			return GL_TRIANGLES;
		default:
			assert(false);
			return 0;
		}
	}
}