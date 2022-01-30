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
		mMaterial = std::make_shared<MaterialGL>();
		aiColor4D diffuse;
		aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
		auto shader = mgrgl->GetShader("pbr");
		mMaterial->SetShader(shader);
		mMaterial->SetShaderParamter("color", Vector4f(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
	}

	MeshGL::MeshGL(void* data, int count, VertexFormat vf)
	{
		auto mgrgl = (GraphicsMgrGL*)GApp->mGraphicsManager;
		Initialize(data, count, vf);
		mMaterial = std::make_shared<MaterialGL>();
		auto shader = mgrgl->GetShader("debug");
		mMaterial->SetShader(shader);
	}

	MeshGL::~MeshGL()
	{
		mPositions = nullptr;
		mNormals = nullptr;
		mTexCoords = nullptr;
		mIndexes = nullptr;
		glDeleteVertexArrays(1, &mVAO);
	}


	void MeshGL::Initialize(aiMesh* mesh) noexcept
	{
		if (!mesh) {
			return;
		}

		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		auto count = mesh->mNumVertices;
		if (mesh->HasPositions()) {
			mPositions = std::make_shared<VertexBufferGL>(mesh->mVertices, count, VertexFormat::VF_P3F, 0);
		}

		if (mesh->HasNormals()) {
			mNormals = std::make_shared<VertexBufferGL>(mesh->mNormals, count, VertexFormat::VF_N3F, 1);
		}

		if (mesh->HasTextureCoords(0)) {
			float* texCoords = (float*)malloc(sizeof(float) * 2 * mesh->mNumVertices);
			for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
				texCoords[k * 2] = mesh->mTextureCoords[0][k].x;
				texCoords[k * 2 + 1] = mesh->mTextureCoords[0][k].y;
			}
			mTexCoords = std::make_shared<VertexBufferGL>(texCoords, count, VertexFormat::VF_T2F, 2);
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
			mIndexes = std::make_shared<IndexBufferGL>(idata, count, IndexFormat::IF_UINT32);
			free(idata);
		}

		if (mesh->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_POINT) {
			mPrimitiveType = PrimitiveType::PT_POINT;
		}
		else if (mesh->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_LINE) {
			mPrimitiveType = PrimitiveType::PT_LINE;
		}
		else if (mesh->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_TRIANGLE) {
			mPrimitiveType = PrimitiveType::PT_TRIANGLE;
		}
		else {
			assert(false);
		}
	}

	void MeshGL::Initialize(void* data, int count, VertexFormat vf) noexcept
	{
		auto mgrgl = (GraphicsMgrGL*)GApp->mGraphicsManager;
		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		mPositions = mgrgl->CreateVertexBuffer(data, count, vf);
		mPrimitiveType = PrimitiveType::PT_LINE;
	}

	void MeshGL::InitializeUI() noexcept
	{
	}

	void MeshGL::InitializeTerrain() noexcept
	{
	}

	void MeshGL::Render(Entity* self) noexcept
	{
		ConstantBuffer cb;

		if (mMeshType == MT_Skybox) {
			BuildMatrixIdentity(cb.world);
			auto world = self->GetWorld();
			auto camera = world->GetCameraSystem()->GetMainCamera()->GetComponent<CameraComponent>();
			cb.view = camera->GetViewMatrixOrigin();
			TransposeInPlace(cb.view);
			cb.projection = camera->GetPerspectiveMatrix();
			TransposeInPlace(cb.projection);
		}
		else if (mMeshType == MT_Model) {
			cb.world = self->GetComponent<TransformComponent>()->GetWorldMatrix();
			TransposeInPlace(cb.world);
			auto world = self->GetWorld();
			auto camera = world->GetCameraSystem()->GetMainCamera()->GetComponent<CameraComponent>();
			cb.view = camera->GetViewMatrix();
			TransposeInPlace(cb.view);
			cb.projection = camera->GetPerspectiveMatrix();
			TransposeInPlace(cb.projection);
		}

		mMaterial->Apply(cb);

		glBindVertexArray(mVAO);
		if (mIndexes) {
			glDrawElements(GetMode(), mIndexes->GetIndexCount(), GL_UNSIGNED_INT, 0x00);
		}
		else {
			glDrawArrays(GetMode(), 0x00, mPositions->GetVertextCount());
		}
	}

	void MeshGL::Render(const Matrix4x4f& world, const Matrix4x4f& view, const Matrix4x4f& projection) noexcept
	{
		ConstantBuffer cb;
		cb.world = world;
		cb.view = view;
		cb.projection = projection;

		mMaterial->Apply(cb);

		glBindVertexArray(mVAO);
		if (mIndexes) {
			glDrawElements(GetMode(), mIndexes->GetIndexCount(), GL_UNSIGNED_INT, 0x00);
		}
		else {
			glDrawArrays(GetMode(), 0x00, mPositions->GetVertextCount());
		}
	}

	GLenum MeshGL::GetMode()
	{
		switch (mPrimitiveType)
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