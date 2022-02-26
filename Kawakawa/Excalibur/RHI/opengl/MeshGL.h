#include "../Mesh.h"
#include "../../Vendor/Glad/include/glad/glad_wgl.h"

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
		GLuint mVAO;
	};

}