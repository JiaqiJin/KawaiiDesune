#include "..//VertexBuffer.h"
#include "../../Vendor/Glad/include/glad/glad_wgl.h"

namespace Excalibur 
{
	class VertexBufferGL : public IVertexBuffer
	{
	public:
		VertexBufferGL(void* data, unsigned int count, VertexFormat vf, int index);
		virtual ~VertexBufferGL();
		virtual void Initialize(void* data, unsigned int count, VertexFormat vf);
		virtual void Finialize();

	private:
		int m_Index;
		GLuint m_VBO;
	};
}