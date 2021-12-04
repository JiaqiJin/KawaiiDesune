#pragma once

#include "../Application.h"
#include "../../Utils/Singleton.h"

namespace Excalibur
{
	class WindowsApplication : public Application 
	{
	public:
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;
		virtual void Tick() noexcept;
		virtual void Render() noexcept;

		HWND GetWindowsHandler() noexcept;

	protected:
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		WindowsApplication() = default;
		void CreateMainWindow();

	private:
		HWND mHWND;

		friend class Singleton<WindowsApplication>;
	};
	typedef Singleton<WindowsApplication> GWindowsApplication;

}