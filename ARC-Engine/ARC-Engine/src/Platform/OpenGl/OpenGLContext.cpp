#include "arc_pch.h"
#include "OpenGLContext.h"
#include "GLFW\glfw3.h"
#include "glad\glad.h"

namespace ARC {

	COpenGLContext::COpenGLContext(GLFWwindow* _WindowHandle) : m_WindowHandle(_WindowHandle)
	{
		ARC_CORE_ASSERT(m_WindowHandle, "Window handle is null")
	}
	void COpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ARC_CORE_ASSERT(status, "Failed to load GLAD!")

		ARC_CORE_INFO("OpenGL Info: ");
		ARC_CORE_INFO("\t Vendor: {0}", glGetString(GL_VENDOR));
		ARC_CORE_INFO("\t Renderer: {0}", glGetString(GL_RENDERER));
		ARC_CORE_INFO("\t Version: {0}", glGetString(GL_VERSION));

	}

	void COpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}