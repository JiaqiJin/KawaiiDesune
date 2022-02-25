#pragma once

#include "../Interface/IResource.h"

namespace Excalibur 
{
	class VertexBuffer : public IRenderResource 
	{
	public:
		virtual void Initialize(void* data, unsigned int count, VertexFormat vf);
		virtual void Finialize();
		virtual unsigned int GetVertexSize(VertexFormat vf);

	public:
		unsigned int m_Count;
		VertexFormat m_VertexFormat;
	};

}