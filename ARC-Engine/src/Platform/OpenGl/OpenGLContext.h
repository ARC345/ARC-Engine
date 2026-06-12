#pragma once
#include "ARC\Renderer\GraphicsContext.h"

struct GLFWwindow;

namespace arc {
	class opengl_context : public graphics_context
	{
	public:
		opengl_context(GLFWwindow* window_handle);
		virtual void init() override;
		virtual void swap_buffers() override;

	private:
		GLFWwindow* window_handle_;
	};
}