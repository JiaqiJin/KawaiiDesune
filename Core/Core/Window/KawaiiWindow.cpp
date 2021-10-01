#include "../Kawaiipch.h"

#include "KawaiiWindow.h"

namespace Kawaii
{
    KawaiiWindow::KawaiiWindow()
        : m_WindowState(WindowState::GetDefault())
    {

    }

    KawaiiWindow::~KawaiiWindow()
    {
    }

    bool KawaiiWindow::Init()
    {
        m_WindowState = WindowState::GetDefault();
        m_Window.Initialize(m_WindowState);

        m_Window.ShowWindow(true);

        return true;
    }

    void KawaiiWindow::Unload()
    {
        m_Window.ShowWindow(false);

        m_Window.Unload();
    }

    void KawaiiWindow::Update()
    {

    }

    const WindowState& KawaiiWindow::GetWindowState()
    {
        return m_WindowState;
    }

    HWND KawaiiWindow::GetHWND() const
    {
        return m_Window.GetHWND();
    }
}