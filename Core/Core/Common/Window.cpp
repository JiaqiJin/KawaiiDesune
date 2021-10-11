#include "../Kawaiipch.h"
#include "Window.h"
#include "../Utils/StringUtil.h"

void Window::Initialize(WindowInitParam const& init)
{
    m_instance = init.instance;
    const std::wstring window_title = Util::ConvertToWide(init.title);
    const float window_width = init.width;
    const float window_height = init.height;
    const LPCWSTR class_name = L"MyWindowClass";

    // Register the Window Class

    WNDCLASSEX wcex{};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_instance;
    //wcex.hIcon = LoadIcon(m_instance, MAKEINTRESOURCE(xxx));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 2);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = class_name;
    wcex.hIconSm = nullptr;

    if (!::RegisterClassExW(&wcex))
        ::MessageBoxA(nullptr, "Window Class Registration Failed!", "Fatal Error!", MB_ICONEXCLAMATION | MB_OK);


    m_handle = ::CreateWindowExW
    (
        0, //WS_EX_CLIENTEDGE,
        class_name,
        window_title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, window_width, window_height,
        nullptr, nullptr, m_instance, nullptr
    );

    if (!m_handle)
    {
        MessageBox(nullptr, L"Window Creation Failed!", L"Fatal Error!", MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    ::SetWindowLong(m_handle, GWL_STYLE, ::GetWindowLong(m_handle, GWL_STYLE) & ~WS_MINIMIZEBOX);

    if (init.maximize) ::ShowWindow(m_handle, SW_SHOWMAXIMIZED);
    else ::ShowWindow(m_handle, SW_SHOWNORMAL);
    ::UpdateWindow(m_handle);
    ::SetFocus(m_handle);
}

LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0ll;

    WindowMsg_t window_data = {};
    window_data.handle = static_cast<void*>(m_handle);
    window_data.instance = static_cast<void*>(m_instance);
    window_data.msg = static_cast<float>(msg);
    window_data.wparam = static_cast<uint64_t>(wParam);
    window_data.lparam = static_cast<int64_t>(lParam);
    window_data.width = static_cast<float>(GetWidth());
    window_data.height = static_cast<float>(GetHeight());

    if (msg == WM_CLOSE || msg == WM_DESTROY)
    {
        ::PostQuitMessage(0);
        return 0;
    }
    else if (msg == WM_DISPLAYCHANGE || msg == WM_SIZE)
    {
        window_data.width = static_cast<float>(lParam & 0xffff);
        window_data.height = static_cast<float>((lParam >> 16) & 0xffff);
    }
    else result = DefWindowProc(hwnd, msg, wParam, lParam);

    if (_msg_callback)
    {
        _msg_callback(window_data);
    }

    return result;
}

float Window::GetWidth()
{
    RECT rect{};
    ::GetClientRect(m_handle, &rect);
    return static_cast<float>(rect.right - rect.left);
}

float Window::GetHeight()
{
    RECT rect{};
    ::GetClientRect(m_handle, &rect);
    return static_cast<float>(rect.bottom - rect.top);
}