#pragma once
#include "../IndexBufferBase.h"
#include <d3d11.h>

namespace Excalibur {

	class IndexBufferD11 : public IndexBufferBase
	{
	public:
		IndexBufferD11(void* data, unsigned int count);
		virtual ~IndexBufferD11();
		virtual void Initialize(void* data, unsigned int count);
		virtual void Finialize() noexcept;

	public:
		ID3D11Buffer*	mIndexBuffer;
	};

}