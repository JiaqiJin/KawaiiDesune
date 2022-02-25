#include "../RenderMesh.h"
#include "../../Vendor/Glad/include/glad/glad_wgl.h"

namespace Excalibur
{
	class RenderMeshGL : public RenderMesh 
	{
	public:
		RenderMeshGL(aiMesh* mesh, const aiScene* world);
		RenderMeshGL(void* data, int count, VertexFormat vf);
		virtual ~RenderMeshGL();
		virtual void Initialize(aiMesh* mesh);
		virtual void Initialize(void* data, int count, VertexFormat vf);
		virtual void Render(World* world, const Matrix4f& worldMatrix);
		virtual void  Finialize();

	private:
		GLenum GetMode();
		GLuint mVAO;
	};

}