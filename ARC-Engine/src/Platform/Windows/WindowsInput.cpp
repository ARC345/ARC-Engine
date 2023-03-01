#include "arc_pch.h"
#include "ARC/Input/Input.h"
#include "ARC/Core/Application.h"
#include "GLFW/glfw3.h"
#include "ARC/Core/Window.h"

namespace ARC {
	bool SInput::IsKeyPressed(int _keycode)
	{
		auto state	= GetKey(_keycode); 
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool SInput::IsMouseButtonPressed(int _button)
	{
		auto window = static_cast<GLFWwindow*>(CApplication::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, _button);
		return state == GLFW_PRESS;
	}

	FVec2 SInput::GetMouseXY()
	{
		auto window = static_cast<GLFWwindow*>(CApplication::Get().GetWindow().GetNativeWindow());
		TVec2<double> mousexy;
		glfwGetCursorPos(window, &mousexy.x, &mousexy.y);
		return FVec2((float)mousexy.x, (float)mousexy.y);
	}

	int SInput::GetKey(int _keycode)
	{
		auto window = static_cast<GLFWwindow*>(CApplication::Get().GetWindow().GetNativeWindow());
		return glfwGetKey(window, _keycode);
	}
}