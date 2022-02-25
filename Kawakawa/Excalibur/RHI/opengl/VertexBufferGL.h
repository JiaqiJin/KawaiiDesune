#include "..//VertexBuffer.h"
#include "../../Vendor/Glad/include/glad/glad_wgl.h"

namespace Excalibur {

	class VertexBufferGL : public VertexBuffer {
	public:
		VertexBufferGL(void* data, unsigned int count, VertexFormat vf, int index);
		virtual ~VertexBufferGL();
		virtual void Initialize(void* data, unsigned int count, VertexFormat vf)  ;
		virtual void Finialize()  ;
	public:
		int mIndex;
		GLuint mVBO;

	};
}