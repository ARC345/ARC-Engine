#include "arc_pch.h"
#include "WindowsWindow.h"
#include "ARC/Core/Log.h"
#include "ARC/Core/Window.h"
#include "ARC\Events\ApplicationEvent.h"
#include "ARC\Events\KeyEvent.h"
#include "ARC\Events\MouseEvent.h"

#include "glad\glad.h"
#include "OpenGl\OpenGLContext.h"

namespace ARC{ 
	static bool s_bGLFWInitialized = false;
	static void GLFWErrorCallback(int _error, const char* _description)
	{
		ARC_CORE_ERROR("GLFW Error ({0}): {1}", _error, _description);
	};
	CWindowsWindow::CWindowsWindow(const SWindowProps& _props)
	{
		Init(_props);
	}

	CWindowsWindow::~CWindowsWindow()
	{
		Shutdown();
	}

	void CWindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void CWindowsWindow::SetVSync(bool _bEnabled)
	{
		glfwSwapInterval(_bEnabled);
		m_Data.bVSync = _bEnabled;
	}

	void* CWindowsWindow::GetNativeWindow() const
	{
		return m_Window;
	}

	CWindow* CWindow::Create(const SWindowProps& _props /*= WindowProps()*/)
	{
		return new CWindowsWindow(_props);
	}

	void CWindowsWindow::Init(const SWindowProps& _props)
	{
		m_Data.Title = _props.Title;
		m_Data.Width = _props.Width;
		m_Data.Height = _props.Height;

		ARC_CORE_INFO("Creating window {0} ({1}, {2})", _props.Title, _props.Width, _props.Height);

		if (!s_bGLFWInitialized) {
			int success = glfwInit();
			ARC_CORE_ASSERT(success, "Could not initialize GLFW")
			glfwSetErrorCallback(GLFWErrorCallback);
			s_bGLFWInitialized = success;
		}

		m_Window = glfwCreateWindow((int)_props.Width, (int)_props.Height, _props.Title.c_str(), nullptr, nullptr);

		m_Context = new COpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* _window, int _width, int _height){
			WindowData& Data = *(WindowData*)glfwGetWindowUserPointer(_window);
			Data.Width = _width;
			Data.Height = _height;

			CWindowResizeEvent event(_width, _height);
			Data.EventCallback(event);
		});
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* _window){
			WindowData& Data = *(WindowData*)glfwGetWindowUserPointer(_window);
			CWindowCloseEvent event;
			Data.EventCallback(event);
		});
		glfwSetKeyCallback(m_Window, [](GLFWwindow* _window, int _key, int _scancode, int _action, int _mods){
			WindowData& Data = *(WindowData*)glfwGetWindowUserPointer(_window);
			switch (_action)
			{
			case GLFW_PRESS: {
				CKeyPressedEvent event(_key, 0);
				Data.EventCallback(event);
				break;
				}
			case GLFW_RELEASE: {
				CKeyReleasedEvent event(_key);
				Data.EventCallback(event);
				break;
				}
			case GLFW_REPEAT: {
				CKeyReleasedEvent event(_key);
				Data.EventCallback(event);
				break;
				}
			}
		});
		glfwSetCharCallback(m_Window, [](GLFWwindow* _window, uint _keycode) {
			WindowData& Data = *(WindowData*)glfwGetWindowUserPointer(_window);
			CKeyTypedEvent _event(_keycode);
			Data.EventCallback(_event);
			});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* _window, int _button, int _action, int _mods) {
			WindowData& Data = *(WindowData*)glfwGetWindowUserPointer(_window);
			switch (_action)
			{
			case GLFW_PRESS: {
				CMouseButtonPressedEvent event(_button);
				Data.EventCallback(event);
				break;
				}
			case GLFW_RELEASE: {
				CMouseButtonReleasedEvent event(_button);
				Data.EventCallback(event);
				break;
				}
			}
		});
		glfwSetScrollCallback(m_Window, [](GLFWwindow* _window, double _xoffset, double _yoffset) {
			WindowData& Data = *(WindowData*)glfwGetWindowUserPointer(_window);
			CMouseScrolledEvent event((float)_xoffset, (float)_yoffset);
			Data.EventCallback(event);
		});
 		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* _window, double _xpos, double _ypos) {
			WindowData& Data = *(WindowData*)glfwGetWindowUserPointer(_window);
			CMouseMovedEvent _event((float)_xpos, (float)_ypos);
			Data.EventCallback(_event);
		});
	}

	void CWindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
}