#include "Application.h"

#include <thread>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "../Core/MemoryManager.h"
#include "../Graphics/GraphicsManager.h"
#include "../HID/InputManager.h"

namespace Kawaii
{
    bool Application::m_Quit = false;

    Application::Application(GfxConfiguration& cfg) 
        : m_Config(cfg)
    {
        m_Logger = spdlog::stdout_color_mt("Application");
#if defined(_DEBUG)
        spdlog::set_level(spdlog::level::debug);
#endif  // _DEBUG
    }

    Application::~Application()
    {

    }

    int Application::Initialize()
    {
        int ret = 0;

        if ((ret = g_MemoryManager->Initialize()) != 0) return ret;
        if ((ret = g_GraphicsManager->Initialize()) != 0) return ret;
        if ((ret = g_InputManager->Initialize()) != 0) return ret;

        return ret;
    }

    void Application::Finalize()
    {
        g_MemoryManager->Finalize();
        g_GraphicsManager->Finalize();
        g_InputManager->Finalize();

        m_Logger->info("Finalized.");
    }

    void Application::Tick()
    {
        g_MemoryManager->Tick();
        g_GraphicsManager->Tick();
        g_InputManager->Tick();
    }

    GfxConfiguration& Application::GetConfiguration()
    {
        return m_Config;
    }

    void Application::SetCommandLineParameters(int argc, char** argv)
    {
        m_ArgSize = argc;
        m_Arg = argv;
    }

    bool Application::IsQuit() { return m_Quit; }
}