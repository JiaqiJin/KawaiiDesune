#include "../IndexBuffer.h"
#include "../../Vendor/Glad/include/glad/glad_wgl.h"


namespace Excalibur
{
	class IndexBufferGL : public IIndexBuffer 
	{
	public:
		IndexBufferGL(void* data, unsigned int count, IndexFormat iformat);
		virtual ~IndexBufferGL();
		virtual void Initialize(void* data, unsigned int count, IndexFormat iformat);
		virtual void Finialize();
	private:
		GLuint m_VEO;
	};
}