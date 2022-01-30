#pragma once

#include "../Interface/IResourceh.h"

namespace Excalibur
{
	enum IndexFormat {
		IF_None = 0,
		IF_UINT16,
		IF_UINT32,
	};

	class IIndexBuffer : public IResouceBase
	{
	public:
		virtual void Initialize(void* data, unsigned int count, IndexFormat iformat) noexcept = 0;

		IIndexBuffer() : mIndexCount(0), mIndexFormat(IndexFormat::IF_None) {}
		unsigned int GetIndexCount() { return mIndexCount; }

	protected:
		unsigned int	mIndexCount;
		IndexFormat		mIndexFormat;
	};

}