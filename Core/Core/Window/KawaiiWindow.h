#pragma once

#include "WindowManager.h"
#include <windows.h>
#include <WindowsX.h>
#include <wrl.h>

namespace Kawaii
{
	class KawaiiWindow
	{
	public:
		KawaiiWindow();
		~KawaiiWindow();

		bool Init();
		void Unload();
		void Update();

		const WindowState& GetWindowState();
		HWND GetHWND() const;

	private:
		WindowManager m_Window;
		WindowState m_WindowState;
	};
}