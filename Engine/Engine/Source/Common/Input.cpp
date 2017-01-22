#include "EnginePCH.h"
#include "Common/Input.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderWindow.h"

namespace E3D
{
	glm::vec2 Input::m_MouseWheel;
	glm::vec2 Input::m_MouseMotion;
	RenderWindow* Input::m_RenderWindow;
	std::pair<int, int> Input::m_InputMode;

	void Input::Init(RenderWindow* window)
	{
		m_RenderWindow = window;
		m_InputMode.first = GLFW_CURSOR;
		m_InputMode.second = GLFW_CURSOR_NORMAL;

		for (uint i = 0; i < (uint)JoystickIndex::LAST; i++)
		{
			const char* jsName = glfwGetJoystickName(i);
			if (jsName == NULL) 
			{
				if (i == 0)
					E3DPrint("No joysticks detected");
				break;
			}
			E3DPrint("Joystick detected(" << i << "): " << jsName);
		}
	}

	void Input::Reset()
	{
		m_MouseMotion.x = 0;
		m_MouseMotion.y = 0;
	}

	bool Input::GetKeyDown(const Key& key)
	{
		return (glfwGetKey(m_RenderWindow->m_Window, (int)key) == GLFW_PRESS);
	}

	bool Input::GetMouseButtonDown(const MouseButton& mouseButton)
	{
		return (glfwGetMouseButton(m_RenderWindow->m_Window, (int)mouseButton) == GLFW_PRESS);
	}

	bool Input::GetKeyUp(const Key& key)
	{
		return (glfwGetKey(m_RenderWindow->m_Window, (int)key) == GLFW_RELEASE);
	}

	bool Input::GetMouseButtonUp(const MouseButton& mouseButton)
	{
		return (glfwGetMouseButton(m_RenderWindow->m_Window, (int)mouseButton) == GLFW_RELEASE);
	}

	bool Input::GetJoystickButtonDown(const JoystickButton& jsButton, const JoystickIndex& jsIndex /*= JoystickIndex::_1*/)
	{
		int count;
		const uchar* axes = glfwGetJoystickButtons((int)jsIndex, &count);
		if (axes == NULL)
			return false;
		return (axes[(int)jsButton] == GLFW_PRESS);
	}

	bool Input::GetJoystickButtonUp(const JoystickButton& jsButton, const JoystickIndex& jsIndex /*= JoystickIndex::_1*/)
	{
		int count;
		const uchar* axes = glfwGetJoystickButtons((int)jsIndex, &count);
		if (axes == NULL)
			return true;
		return (axes[(int)jsButton] == GLFW_RELEASE);
	}

	float Input::GetJoystickAxis(const JoystickAxis& jsAxis, const JoystickIndex& jsIndex /*= JoystickIndex::_1*/)
	{
		int count;
		const float* axes = glfwGetJoystickAxes((int)jsIndex, &count);
		if (axes == NULL)
			return 0.0f;
		return axes[(int)jsAxis];
	}

	const glm::vec2& Input::GetMouseWheel()
	{
		return m_MouseWheel;
	}

	glm::vec2 Input::GetMousePosition()
	{
		double x, y;
		glfwGetCursorPos(m_RenderWindow->m_Window, &x, &y);
		return glm::vec2((float)x, (float)y);
	}

	void Input::DisableCursor()
	{
		if (m_InputMode.first = GLFW_CURSOR && m_InputMode.second != GLFW_CURSOR_DISABLED) 
		{
			glfwSetInputMode(m_RenderWindow->m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			m_InputMode.first = GLFW_CURSOR;
			m_InputMode.second = GLFW_CURSOR_DISABLED;
		}
	}

	void Input::EnableCursor()
	{
		if (m_InputMode.first = GLFW_CURSOR && m_InputMode.second != GLFW_CURSOR_NORMAL) 
		{
			glfwSetInputMode(m_RenderWindow->m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			m_InputMode.first = GLFW_CURSOR;
			m_InputMode.second = GLFW_CURSOR_NORMAL;
		}
	}

	void Input::CenterCursor()
	{
		glfwSetCursorPos(m_RenderWindow->m_Window, m_RenderWindow->GetWidth() * 0.5f, m_RenderWindow->GetHeight() * 0.5f);
	}

	void Input::UnlockCursor()
	{
		if (m_InputMode.first = GLFW_CURSOR && m_InputMode.second != GLFW_CURSOR_NORMAL) 
		{
			glfwSetInputMode(m_RenderWindow->m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			m_InputMode.first = GLFW_CURSOR;
			m_InputMode.second = GLFW_CURSOR_NORMAL;
		}
	}

	void Input::ShowCursor()
	{
		if (m_InputMode.first = GLFW_CURSOR && m_InputMode.second != GLFW_CURSOR_NORMAL)
		{
			glfwSetInputMode(m_RenderWindow->m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			m_InputMode.first = GLFW_CURSOR;
			m_InputMode.second = GLFW_CURSOR_NORMAL;
		}
	}

	void Input::HideCursor()
	{
		if (m_InputMode.first = GLFW_CURSOR && m_InputMode.second != GLFW_CURSOR_HIDDEN) 
		{
			glfwSetInputMode(m_RenderWindow->m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			m_InputMode.first = GLFW_CURSOR;
			m_InputMode.second = GLFW_CURSOR_HIDDEN;
		}
	}

	void Input::RefreshContext()
	{
		glfwSetInputMode(m_RenderWindow->m_Window, m_InputMode.first, m_InputMode.second);
	}

};