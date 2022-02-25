#pragma once
#include "IModule.h"

namespace Excalibur {

	class ISystem : public IModule
	{
	public:
		virtual int Initialize() = 0;
		virtual void Finalize() = 0;

	public:
		virtual bool IsActive() { return mIsActive; }
		virtual void Activate() { mIsActive = true; }
		virtual void Deactivate() { mIsActive = false; }

	public:
		ISystem() : mIsActive(true){}

	private:
		bool mIsActive;
	};

	class ITickableSystem : public ISystem
	{
	public:
		virtual int Initialize() = 0;
		virtual void Finalize() = 0;
		virtual void Tick() = 0;
	};
}