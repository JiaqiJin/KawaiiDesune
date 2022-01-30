#pragma once

#include "../VertexBufferBase.h"
#include <d3d11.h>

namespace Excalibur 
{
	class VertexBufferD11 : public VertexBufferBase
	{
	public:
		VertexBufferD11(void* data, unsigned int count, VertexFormat vf);
		virtual ~VertexBufferD11();
		virtual void Initialize(void* data, unsigned int count, VertexFormat vf);

	public:
		ID3D11Buffer *mVertexBuffer;
	};
}