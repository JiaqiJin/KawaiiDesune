#pragma once

#include <assert.h>

namespace Excalibur 
{

	class IModule
	{
	public:
		virtual int Initialize() = 0;
		virtual void Finalize() = 0;

	};

	class ITickableModule : public IModule
	{
		virtual void Tick() = 0;
	};
}