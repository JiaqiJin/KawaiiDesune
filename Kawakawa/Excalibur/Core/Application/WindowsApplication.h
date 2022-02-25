#pragma once

#include "Application.h"
#include "../../Utils/Singleton.h"
#include <Windows.h>
#include "../../Vendor/Glad/include/glad/glad_wgl.h"

namespace Excalibur 
{
	class WindowsApplication : public Application
	{
	public:
		virtual int Initialize();
		virtual void Finalize();
		virtual void Tick();
		virtual void Render();

		HWND GetWindowsHandler();
	
	protected:
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		WindowsApplication() = default;
		void CreateMainWindow();

		void LoadWGL();
		void CreateGLContext();

	private:
		HWND mHWND;
		HGLRC m_RenderContext;
		friend class Singleton<WindowsApplication>;

		bool mUseOpengl;
	};

	typedef Singleton<WindowsApplication> GWindowsApplication;
}