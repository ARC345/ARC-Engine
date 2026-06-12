#include "arc_pch.h"
#include "FrameBuffer.h"
#include "Platform\OpenGl\OpenGLFrameBuffer.h"

namespace arc {

	ref<framebuffer> framebuffer::create(const frame_buffer_specification& spec)
	{
		switch (renderer::get_current_api())
		{
		case renderer_api::renderer_api::None:
			ARC_CORE_ASSERT(false, "Selected renderer API is not supported (renderer_api::None)");
			return nullptr;
		case renderer_api::renderer_api::OpenGL:
			return create_ref<opengl_framebuffer>(spec);
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;
	}

}