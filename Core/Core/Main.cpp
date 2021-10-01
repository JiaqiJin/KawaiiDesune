#include <iostream>

#include "Window/KawaiiWindow.h"
#include "Window/WindowManager.h"
#include "Common/RenderBackendImpl.h"
#include "Common/RenderBacken.h"

int RealMain(int argc, wchar_t* argv[])
{
	Kawaii::RenderBackend* renderer = Kawaii::RenderBackend::CreateRenderBackend();
    renderer->Initialize();
   /* Kawaii::WindowState winState;
    winState.height = 800;
    winState.width = 800;
    winState.x = 800;
    winState.y = 800;
    winState.windowMode = Kawaii::WindowState::WindowMode::Windowed;
    Kawaii::WindowManager windowManager;
    windowManager.Initialize(winState);
    windowManager.ShowWindow(true);*/
    ::GetAsyncKeyState(VK_ESCAPE);
    while (true)
    {
        if (::GetAsyncKeyState(VK_ESCAPE))
            break;
    }

    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR pCmdLine, int nCmdShow)
{
	int argc;
	LPWSTR* argv = ::CommandLineToArgvW(pCmdLine, &argc);
	return RealMain(argc, argv);
}