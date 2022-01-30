#include "WindowsApplication.h"
#include "../../RHI/OpenGL/GraphicsMgrGL.h"

#include "../../Vendor/Glad/include/glad/glad_wgl.h"
#include <windowsx.h>

namespace Excalibur
{
	static LRESULT CALLBACK TmpWndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uiMsg)
		{
		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, uiMsg, wParam, lParam);
		}

		return 0;
	}

	LRESULT WindowsApplication::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WindowsApplication* pThis;
		pThis = reinterpret_cast<WindowsApplication*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		switch (message) {
		case WM_CREATE: {
			LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			pThis->Quit();
			return 0;
		}
		}

		// Handle any messages the switch statement didn't.
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	int WindowsApplication::Initialize() noexcept {
		CHECK_APPLICATION_INIT(Application::Initialize());
		CreateMainWindow();

		mMemoryMgr = new MemoryManager();
		mMemoryMgr->Initialize();
		bool openGL = true;
		if (openGL)
		{
			LoadWGL();
			CreateGLContext();
			mGraphicsManager = new GraphicsMgrGL();
			auto mgr = (GraphicsMgrGL*)mGraphicsManager;
			mgr->Initialize();
		}
		/*else
		{
			mGraphicsManager = new GraphicsMgrD12();
			auto mgr = (GraphicsMgrD12*)mGraphicsManager;
			mgr->InitializeWithWindow(mHWND);
		}*/

		mWorld = new World(this);
		mWorld->Initialize();

		return 0;
	}

	void WindowsApplication::Tick() noexcept {
		Application::Tick();
		MSG msg = {};
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		mWorld->Tick();
		Render();
	}

	void WindowsApplication::Render() noexcept {
		mWorld->Render();
		mGraphicsManager->Present();
		auto m_hDC = GetDC(mHWND);
		SwapBuffers(m_hDC);
	}

	HWND WindowsApplication::GetWindowsHandler() noexcept
	{
		return mHWND;
	}

	void WindowsApplication::CreateMainWindow()
	{
		HINSTANCE hInstance = GetModuleHandle(NULL);
		WNDCLASSEX windowClass = { 0 };
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		windowClass.lpfnWndProc = WindowProc;
		windowClass.hInstance = hInstance;
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.lpszClassName = L"Excalibur";
		RegisterClassEx(&windowClass);

		RECT windowRect = { 0, 0, 1024, 768 };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		// Create the window and store a handle to it.
		mHWND = CreateWindow(
			windowClass.lpszClassName,
			L"Excalibur",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			nullptr,        // We have no parent window.
			nullptr,        // We aren't using menus.
			hInstance,
			this);

		// Initialize the sample. OnInit is defined in each child-implementation of DXSample.
		ShowWindow(mHWND, 5);
	}

	void WindowsApplication::Finalize() noexcept
	{
		mWorld->Finalize();
		mGraphicsManager->Finalize();
		mMemoryMgr->Finalize();
	}

	void WindowsApplication::LoadWGL() 
	{
		int result = 0;
		DWORD Style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		WNDCLASSEX WndClassEx;
		memset(&WndClassEx, 0, sizeof(WNDCLASSEX));
		HINSTANCE hInstance = GetModuleHandle(NULL);
		WndClassEx.cbSize = sizeof(WNDCLASSEX);
		WndClassEx.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		WndClassEx.lpfnWndProc = TmpWndProc;
		WndClassEx.hInstance = hInstance;
		WndClassEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		WndClassEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		WndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
		WndClassEx.lpszClassName = L"InitWindow";
		RegisterClassEx(&WndClassEx);
		HWND TemphWnd = CreateWindowEx(WS_EX_APPWINDOW, WndClassEx.lpszClassName, L"InitWindow", Style, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cRedBits = 8;
		pfd.cGreenBits = 8;
		pfd.cBlueBits = 8;
		pfd.cAlphaBits = 8;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;

		HDC TemphDC = GetDC(TemphWnd);
		// Set a temporary default pixel format.
		int nPixelFormat = ChoosePixelFormat(TemphDC, &pfd);
		assert(nPixelFormat != 0);

		result = SetPixelFormat(TemphDC, nPixelFormat, &pfd);
		assert(result == 1);

		// Create a temporary rendering context.
		m_RenderContext = wglCreateContext(TemphDC);
		assert(result == 1);

		// Set the temporary rendering context as the current rendering context for this window.
		result = wglMakeCurrent(TemphDC, m_RenderContext);
		assert(result == 1);

		auto r = gladLoadWGL(TemphDC);
		assert(r != 0);

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_RenderContext);
		ReleaseDC(TemphWnd, TemphDC);
		DestroyWindow(TemphWnd);
	}

	void WindowsApplication::CreateGLContext() 
	{
		int nPixelFormat;
		UINT numFormats;
		PIXELFORMATDESCRIPTOR pfd;
		int result;

		auto m_hDC = GetDC(mHWND);
		if (GLAD_WGL_ARB_pixel_format && GLAD_WGL_ARB_multisample && GLAD_WGL_ARB_create_context)
		{
			// enable MSAA
			const int attributes[] = {
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
				WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
				WGL_COLOR_BITS_ARB,     (int)24,
				WGL_RED_BITS_ARB,		(int)8,
				WGL_GREEN_BITS_ARB,		(int)8,
				WGL_BLUE_BITS_ARB,		(int)8,
				WGL_ALPHA_BITS_ARB,		(int)8,
				WGL_DEPTH_BITS_ARB,     (int)24,
				WGL_STENCIL_BITS_ARB,   (int)8,
				WGL_SAMPLE_BUFFERS_ARB, 1,
				WGL_SAMPLES_ARB,        4,  // 4x MSAA
				0
			};

			bool fail = wglChoosePixelFormatARB(m_hDC, attributes, nullptr, 1, &nPixelFormat, &numFormats) < 0 || numFormats == 0;
			assert(!fail);

			result = SetPixelFormat(m_hDC, nPixelFormat, &pfd);
			assert(result == 1);

			const int context_attributes[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
				WGL_CONTEXT_MINOR_VERSION_ARB, 1,
				WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
				WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
				0
			};

			m_RenderContext = wglCreateContextAttribsARB(m_hDC, 0, context_attributes);
			assert(m_RenderContext);

			result = wglMakeCurrent(m_hDC, m_RenderContext);
			assert(result == 1);

		}
		else 
		{
			// Set pixel format.
			int nPixelFormat = ChoosePixelFormat(m_hDC, &pfd);
			assert(nPixelFormat != 0);

			result = SetPixelFormat(m_hDC, nPixelFormat, &pfd);
			assert(result == 1);

			// Create rendering context.
			m_RenderContext = wglCreateContext(m_hDC);
			assert(m_RenderContext);

			// Set the rendering context as the current rendering context for this window.
			result = wglMakeCurrent(m_hDC, m_RenderContext);
			assert(result == 1);
		}
	}
}