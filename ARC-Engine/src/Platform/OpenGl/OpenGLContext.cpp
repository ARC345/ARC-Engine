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
		ARC_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_WindowHandle);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ARC_CORE_ASSERT(status, "Failed to load GLAD!")

		ARC_CORE_INFO("OpenGL Info: ");
		ARC_CORE_INFO("\t Vendor: {0}", glGetString(GL_VENDOR));
		ARC_CORE_INFO("\t Renderer: {0}", glGetString(GL_RENDERER));
		ARC_CORE_INFO("\t Version: {0}", glGetString(GL_VERSION));

		#ifdef ARC_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		ARC_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "ARC requires at least OpenGL version 4.5!");
		#endif
	}

	void COpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}