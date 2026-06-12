#include "arc_pch.h"
#include "WindowsWindow.h"
#include "ARC/Core/Log.h"
#include "ARC/Core/Window.h"
#include "ARC\Events\ApplicationEvent.h"
#include "ARC\Events\KeyEvent.h"
#include "ARC\Events\MouseEvent.h"

#pragma warning(push, 0)
#include "glad\glad.h"
#pragma warning(pop)
#include "Platform\OpenGl\OpenGLContext.h"
#include "ARC/Core/Macros.h"

namespace arc{ 
	static bool s_bGLFWInitialized = false;
	static void GLFWErrorCallback(int _error, const char* _description)
	{
		ARC_CORE_ERROR("GLFW Error ({0}): {1}", _error, _description);
	};
	windows_window::windows_window(const window_props& _props)
	{
		ARC_PROFILE_FUNCTION();
		init(_props);
	}

	windows_window::~windows_window()
	{
		shutdown();
	}

	void windows_window::on_update()
	{
		glfwPollEvents();
		context_->swap_buffers();
	}

	void windows_window::set_vsync(bool _bEnabled)
	{
		glfwSwapInterval(_bEnabled);
		data_.bVSync = _bEnabled;
	}

	void* windows_window::get_native_window() const
	{
		return window_;
	}

	window* window::create(const window_props& _props /*= WindowProps()*/)
	{
		return new windows_window(_props);
	}

	void windows_window::init(const window_props& _props)
	{
		ARC_PROFILE_FUNCTION();
		data_.Title = _props.Title;
		data_.Width = _props.Width;
		data_.Height = _props.Height;

		ARC_CORE_INFO("Creating window {0} ({1}, {2})", _props.Title, _props.Width, _props.Height);

		if (!s_bGLFWInitialized) {
			int success = glfwInit();
			ARC_CORE_ASSERT(success, "Could not initialize GLFW")
			glfwSetErrorCallback(GLFWErrorCallback);
			s_bGLFWInitialized = success;
		}

		window_ = glfwCreateWindow((int)_props.Width, (int)_props.Height, _props.Title.c_str(), nullptr, nullptr);

		context_ = new opengl_context(window_);
		context_->init();

		glfwSetWindowUserPointer(window_, &data_);
		set_vsync(true);

		glfwSetWindowSizeCallback(window_, [](GLFWwindow* _window, int _width, int _height){
			window_data& Data = *(window_data*)glfwGetWindowUserPointer(_window);
			Data.Width = _width;
			Data.Height = _height;

			window_resize_event event(_width, _height);
			Data.EventCallback(event);
		});
		glfwSetWindowCloseCallback(window_, [](GLFWwindow* _window){
			window_data& Data = *(window_data*)glfwGetWindowUserPointer(_window);
			window_close_event event;
			Data.EventCallback(event);
		});
		glfwSetKeyCallback(window_, [](GLFWwindow* _window, int _key, int _scancode, int _action, int _mods){
			window_data& Data = *(window_data*)glfwGetWindowUserPointer(_window);
			switch (_action)
			{
			case GLFW_PRESS: {
				key_pressed_event event(_key, 0);
				Data.EventCallback(event);
				break;
				}
			case GLFW_RELEASE: {
				key_released_event event(_key);
				Data.EventCallback(event);
				break;
				}
			case GLFW_REPEAT: {
				key_released_event event(_key);
				Data.EventCallback(event);
				break;
				}
			}
		});
		glfwSetCharCallback(window_, [](GLFWwindow* _window, uint _keycode) {
			window_data& Data = *(window_data*)glfwGetWindowUserPointer(_window);
			key_typed_event _event(_keycode);
			Data.EventCallback(_event);
			});
		glfwSetMouseButtonCallback(window_, [](GLFWwindow* _window, int _button, int _action, int _mods) {
			window_data& Data = *(window_data*)glfwGetWindowUserPointer(_window);
			switch (_action)
			{
			case GLFW_PRESS: {
				mouse_button_pressed_event event(_button);
				Data.EventCallback(event);
				break;
				}
			case GLFW_RELEASE: {
				mouse_button_released_event event(_button);
				Data.EventCallback(event);
				break;
				}
			}
		});
		glfwSetScrollCallback(window_, [](GLFWwindow* _window, double _xoffset, double _yoffset) {
			window_data& Data = *(window_data*)glfwGetWindowUserPointer(_window);
			mouse_scrolled_event event((float)_xoffset, (float)_yoffset);
			Data.EventCallback(event);
		});
 		glfwSetCursorPosCallback(window_, [](GLFWwindow* _window, double _xpos, double _ypos) {
			window_data& Data = *(window_data*)glfwGetWindowUserPointer(_window);
			mouse_moved_event _event((float)_xpos, (float)_ypos);
			Data.EventCallback(_event);
		});
	}

	void windows_window::shutdown()
	{
		glfwDestroyWindow(window_);
	}
}