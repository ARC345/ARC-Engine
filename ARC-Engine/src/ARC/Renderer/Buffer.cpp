#include "arc_pch.h"
#include "Buffer.h"
#include "..\Renderer\Renderer.h"
#include "Platform\OpenGl\OpenGLBuffer.h"
#include "..\Core\Core.h"

namespace arc {

	ref<vertex_buffer> vertex_buffer::create(float* vertices, uint32_t size)
	{
		switch (renderer::get_current_api())
		{
			case renderer_api::renderer_api::None:
				ARC_CORE_ASSERT(false, "Selected renderer API is not supported (renderer_api::None)");
				return nullptr;
			case renderer_api::renderer_api::OpenGL:
				return std::make_shared<opengl_vertex_buffer>(vertices, size);
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;
	}

	ref<vertex_buffer> vertex_buffer::create(uint32_t size)
	{
		switch (renderer::get_current_api())
		{
		case renderer_api::renderer_api::None:
			ARC_CORE_ASSERT(false, "Selected renderer API is not supported (renderer_api::None)");
			return nullptr;
		case renderer_api::renderer_api::OpenGL:
			return std::make_shared<opengl_vertex_buffer>(size);
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;
	}

	ref<index_buffer> index_buffer::create(uint32_t* indices, uint32_t count)
	{
		switch (renderer::get_current_api())
		{
		case renderer_api::renderer_api::None:
			ARC_CORE_ASSERT(false, "Selected renderer API is not supported (renderer_api::None)");
			return nullptr;
		case renderer_api::renderer_api::OpenGL:
			return std::make_shared<opengl_index_buffer>(indices, count);
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;
	}
}