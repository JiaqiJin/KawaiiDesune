#pragma once

#include "../Interface/IResourceh.h"

namespace Excalibur
{
	class IIndexBuffer : public IResouceBase
	{
	public:
		virtual void Initialize(void* data, unsigned int count, IndexFormat iformat)
		{
			mCount = count;
			mIndexFormat = iformat;
		}

		virtual void Finialize() = 0;

		unsigned int GetCount() { return mCount; }
		IndexFormat	GetIndexFormat() { return mIndexFormat; }
	protected:
		unsigned int	mCount;
		IndexFormat		mIndexFormat;
	};

}