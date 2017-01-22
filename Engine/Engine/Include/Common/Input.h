#pragma once

#include "Common/InputEnums.h"

namespace E3D
{
	class RenderWindow;

	class Input
	{
		friend class Game;
		friend class RenderWindow;
	public:
		static bool GetKeyDown(const Key& key);
		static bool GetMouseButtonDown(const MouseButton& mouseButton);
		static bool GetKeyUp(const Key& key);
		static bool GetMouseButtonUp(const MouseButton& mouseButton);
		static bool GetJoystickButtonDown(const JoystickButton& jsButton, const JoystickIndex& jsIndex = JoystickIndex::_1);
		static bool GetJoystickButtonUp(const JoystickButton& jsButton, const JoystickIndex& jsIndex = JoystickIndex::_1);
		static float GetJoystickAxis(const JoystickAxis& jsAxis, const JoystickIndex& jsIndex = JoystickIndex::_1);
		static const glm::vec2& GetMouseWheel();
		static glm::vec2 GetMousePosition();
		static void DisableCursor();
		static void EnableCursor();
		static void CenterCursor();
		static void UnlockCursor();
		static void HideCursor();
		static void ShowCursor();

	private:
		static void Init(RenderWindow* window);
		static void Reset();
		static void RefreshContext();

		static std::pair<int, int> m_InputMode;
		static glm::vec2 m_MouseWheel;
		static glm::vec2 m_MouseMotion;
		static RenderWindow* m_RenderWindow;
	};
};