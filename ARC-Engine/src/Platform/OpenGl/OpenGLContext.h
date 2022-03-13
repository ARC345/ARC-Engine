#pragma once
#include "ARC\Renderer\GraphicsContext.h"

struct GLFWwindow;

namespace ARC {
	class COpenGLContext : public CGraphicsContext
	{
	public:
		COpenGLContext(GLFWwindow* _WindowHandle);
		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}