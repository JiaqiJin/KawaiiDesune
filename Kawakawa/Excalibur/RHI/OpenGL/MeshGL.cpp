#include "MeshGL.h"
#include "VertexBufferGL.h"
#include "IndexBufferGL.h"
#include "ShaderGL.h"

namespace Excalibur
{
	MeshGL::MeshGL()
	{

	}

	MeshGL::MeshGL(aiMesh* mesh, const aiScene* world)
	{

		Initialize(mesh);
	}

	MeshGL::~MeshGL()
	{
		m_Position = nullptr;
		m_Normal = nullptr;
		m_Indexes = nullptr;
	}

	void MeshGL::Initialize(aiMesh* mesh)
	{
		if (!mesh)
			return;

		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		auto count = mesh->mNumVertices;
		if (mesh->HasPositions()) {
			m_Position = std::make_shared<VertexBufferGL>(mesh->mVertices, count, VertexFormat::VF_3F, 0);
		}

		if (mesh->HasFaces())
		{
			unsigned int count = 3 * mesh->mNumFaces;
			unsigned int* idata = new unsigned int[count];
			for (int i = 0; i < mesh->mNumFaces; ++i)
			{
				auto face = mesh->mFaces[i];
				idata[i * 3] = face.mIndices[0];
				idata[i * 3 + 1] = face.mIndices[1];
				idata[i * 3 + 2] = face.mIndices[2];
			}
			m_Indexes = std::make_shared<IndexBufferGL>(idata, count);
			free(idata);
		}
	}

	void MeshGL::Render()
	{
		// TODO

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