#pragma once

namespace Excalibur {

#define CHECK_APPLICATION_INIT(result)	if(result!=0){ exit(result);}
	
	class IApplication 
	{
	public:
		virtual int Initialize()  = 0;
		virtual void Run() = 0;
		virtual void Tick() = 0;
		virtual void Render() = 0;
		virtual void Finalize() = 0;
		virtual void Quit() = 0;
		virtual bool IsQuit() = 0;
	};
}