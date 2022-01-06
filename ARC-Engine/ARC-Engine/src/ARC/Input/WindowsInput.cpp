#include "arc_pch.h"
#include "WindowsInput.h"
#include "ARC\Application.h"
#include "GLFW\glfw3.h"
#include "ARC\Window.h"

namespace ARC {
	CInput* CInput::s_Instance = new CWindowsInput();
	bool CWindowsInput::IsKeyPressedImpl(int _keycode) const
	{
		auto state	= GetKey(_keycode); 
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool CWindowsInput::IsMouseButtonPressedImpl(int _button) const
	{
		auto window = static_cast<GLFWwindow*>(Core::CApplication::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, _button);
		return state == GLFW_PRESS;
	}

	float CWindowsInput::GetMouseXImpl() const
	{
		auto window = static_cast<GLFWwindow*>(Core::CApplication::Get().GetWindow().GetNativeWindow());
		double mousex, mousey;
		glfwGetCursorPos(window, &mousex, &mousey);
		return (float)mousex;
	}

	float CWindowsInput::GetMouseYImpl() const
	{
		auto window = static_cast<GLFWwindow*>(Core::CApplication::Get().GetWindow().GetNativeWindow());
		double mousex, mousey;
		glfwGetCursorPos(window, &mousex, &mousey);
		return (float)mousey;
	}

	TVec2<> CWindowsInput::GetMouseXYImpl() const
	{
		auto window = static_cast<GLFWwindow*>(Core::CApplication::Get().GetWindow().GetNativeWindow());
		TVec2<double> mousexy;
		glfwGetCursorPos(window, &mousexy[0], &mousexy[1]);
		return TVec2<>((float)mousexy.x(), (float)mousexy.y());
	}

	int CWindowsInput::GetKeyImpl(int _keycode)
	{
		auto window = static_cast<GLFWwindow*>(Core::CApplication::Get().GetWindow().GetNativeWindow());
		return glfwGetKey(window, _keycode);
	}
}