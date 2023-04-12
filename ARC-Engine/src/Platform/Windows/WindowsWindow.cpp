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

namespace ARC{ 
	static bool s_bGLFWInitialized = false;
	static void GLFWErrorCallback(int _error, const char* _description)
	{
		ARC_CORE_ERROR("GLFW Error ({0}): {1}", _error, _description);
	};
	CWindowsWindow::CWindowsWindow(const SWindowProps& _props)
	{
		ARC_PROFILE_FUNCTION();
		Init(_props);
	}

	CWindowsWindow::~CWindowsWindow()
	{
		Shutdown();
	}

	void CWindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		mContext->SwapBuffers();
	}

	void CWindowsWindow::SetVSync(bool _bEnabled)
	{
		glfwSwapInterval(_bEnabled);
		mData.bVSync = _bEnabled;
	}

	void* CWindowsWindow::GetNativeWindow() const
	{
		return mWindow;
	}

	CWindow* CWindow::Create(const SWindowProps& _props /*= WindowProps()*/)
	{
		return new CWindowsWindow(_props);
	}

	void CWindowsWindow::Init(const SWindowProps& _props)
	{
		ARC_PROFILE_FUNCTION();
		mData.Title = _props.Title;
		mData.Width = _props.Width;
		mData.Height = _props.Height;

		ARC_CORE_INFO("Creating window {0} ({1}, {2})", _props.Title, _props.Width, _props.Height);

		if (!s_bGLFWInitialized) {
			int success = glfwInit();
			ARC_CORE_ASSERT(success, "Could not initialize GLFW")
			glfwSetErrorCallback(GLFWErrorCallback);
			s_bGLFWInitialized = success;
		}

		mWindow = glfwCreateWindow((int)_props.Width, (int)_props.Height, _props.Title.c_str(), nullptr, nullptr);

		mContext = new COpenGLContext(mWindow);
		mContext->Init();

		glfwSetWindowUserPointer(mWindow, &mData);
		SetVSync(true);

		glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* _window, int _width, int _height){
			SWindowData& Data = *(SWindowData*)glfwGetWindowUserPointer(_window);
			Data.Width = _width;
			Data.Height = _height;

			CWindowResizeEvent event(_width, _height);
			Data.EventCallback(event);
		});
		glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* _window){
			SWindowData& Data = *(SWindowData*)glfwGetWindowUserPointer(_window);
			CWindowCloseEvent event;
			Data.EventCallback(event);
		});
		glfwSetKeyCallback(mWindow, [](GLFWwindow* _window, int _key, int _scancode, int _action, int _mods){
			SWindowData& Data = *(SWindowData*)glfwGetWindowUserPointer(_window);
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
		glfwSetCharCallback(mWindow, [](GLFWwindow* _window, TUInt _keycode) {
			SWindowData& Data = *(SWindowData*)glfwGetWindowUserPointer(_window);
			CKeyTypedEvent _event(_keycode);
			Data.EventCallback(_event);
			});
		glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* _window, int _button, int _action, int _mods) {
			SWindowData& Data = *(SWindowData*)glfwGetWindowUserPointer(_window);
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
		glfwSetScrollCallback(mWindow, [](GLFWwindow* _window, double _xoffset, double _yoffset) {
			SWindowData& Data = *(SWindowData*)glfwGetWindowUserPointer(_window);
			CMouseScrolledEvent event((float)_xoffset, (float)_yoffset);
			Data.EventCallback(event);
		});
 		glfwSetCursorPosCallback(mWindow, [](GLFWwindow* _window, double _xpos, double _ypos) {
			SWindowData& Data = *(SWindowData*)glfwGetWindowUserPointer(_window);
			CMouseMovedEvent _event((float)_xpos, (float)_ypos);
			Data.EventCallback(_event);
		});
	}

	void CWindowsWindow::Shutdown()
	{
		glfwDestroyWindow(mWindow);
	}
}