#include "arc_pch.h"
#include "ARC/Input/Input.h"
#include "ARC/Core/Application.h"
#include "GLFW/glfw3.h"
#include "ARC/Core/Window.h"

namespace arc {
	bool input::is_key_pressed(int _keycode)
	{
		auto state	= get_key(_keycode); 
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool input::is_mouse_button_pressed(int _button)
	{
		auto window = static_cast<GLFWwindow*>(application::Get().get_window().get_native_window());
		auto state = glfwGetMouseButton(window, _button);
		return state == GLFW_PRESS;
	}

	vec2 input::get_mouse_xy()
	{
		auto window = static_cast<GLFWwindow*>(application::Get().get_window().get_native_window());
		vec2<double> mousexy;
		glfwGetCursorPos(window, &mousexy.x, &mousexy.y);
		return vec2((float)mousexy.x, (float)mousexy.y);
	}

	int input::get_key(int _keycode)
	{
		auto window = static_cast<GLFWwindow*>(application::Get().get_window().get_native_window());
		return glfwGetKey(window, _keycode);
	}
}