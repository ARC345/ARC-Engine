#include "arc_pch.h"
#include "RenderCommand.h"

#include "Platform/OpenGl/OpenGLRendererAPI.h"

namespace arc {
	renderer_api* render_command::sRendererAPI = new opengl_renderer_api; 
}