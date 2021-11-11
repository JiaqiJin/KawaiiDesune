#pragma once

#include "InputEvent.h"
#include "../Interface/IRuntimeModule.h"

#include <variant>
#include <unordered_map>

namespace Kawaii
{
	class InputManager : public IRuntimeModule
	{
	public:
		using UserDefAction = int;
		int  Initialize() final;
		void Finalize() final;
		void Tick() final;

	private:
		std::array<KeyState, static_cast<size_t>(VirtualKeyCode::NUM)> m_KeyState;
		MouseState                                                     m_MouseState;
		std::unordered_map<UserDefAction, std::variant<VirtualKeyCode, MouseEvent>> m_UserMap;
	};

	extern std::unique_ptr<InputManager> g_InputManager;
}