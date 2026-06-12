#include "arc_pch.h"
#include "VertexArray.h"
#include "..\..\Platform\OpenGl\OpenGLVertexArray.h"
#include "Renderer.h"

namespace arc {
	ref<vertex_array> vertex_array::create()
	{
		switch (renderer::get_current_api())
		{
		case renderer_api::renderer_api::None:
			ARC_CORE_ASSERT(false, "Selected renderer API is not supported (renderer_api::None)");
			return nullptr;
		case renderer_api::renderer_api::OpenGL:
			return std::make_shared<opengl_vertex_array>();
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;
	}
}