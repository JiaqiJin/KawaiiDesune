#include "InputManager.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "../Platform/Application.h"

namespace Kawaii
{
	std::unique_ptr<InputManager> g_InputManager = std::make_unique<InputManager>();

    int InputManager::Initialize() 
    {
        m_Logger = spdlog::stdout_color_mt("InputManager");
        m_Logger->info("Initialize...");

        auto& config = g_App->GetConfiguration();

        return 0;
    }

    void InputManager::Finalize()
    {
        m_Logger->info("Finalized.");
        m_Logger = nullptr;
    }

    void InputManager::Tick()
    {
        for (auto&& state : m_KeyState)
            state.previous = state.current;

        m_MouseState.lastPos = m_MouseState.currPos;

        g_App->UpdateInputEvent();
    }
}