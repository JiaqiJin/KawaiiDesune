#pragma once

#include <cstdint>
#include <array>
#include <unordered_map>

namespace KAWAII
{
	enum class KeyCode : uint32_t
	{
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,
	};

	class Input
	{
	public:
		Input();

		bool GetKey(KeyCode code) { return m_keys[code]; }
		bool isKeyDown(KeyCode code) { return GetKey(code) && !m_prevKey[code]; }
		bool isKeyUp(KeyCode code) { return !GetKey(code) && m_prevKey[code]; }

	private:
		std::unordered_map<KeyCode, bool> m_keys;
		std::unordered_map<KeyCode, bool> m_prevKey;

		float mousePox_X = 0.0f;
		float mousePosY = 0.0f;
	};
}