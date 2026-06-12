#include "arc_pch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform\OpenGl\OpenGLTexture.h"
#include "ARC\Core\Core.h"

namespace arc {

	ref<texture_2d> texture_2d::create(const std::filesystem::path& pPath, bool pbManualClear)
	{
		switch (renderer::get_current_api())
		{
		case renderer_api::renderer_api::None:
			ARC_CORE_ASSERT(false, "Selected renderer API is not supported (renderer_api::None)");
			return nullptr;
		case renderer_api::renderer_api::OpenGL:
			return create_ref<opengl_texture_2d>(pPath, pbManualClear);
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;
	}

	ref<texture_2d> texture_2d::create(const vec2<uint32_t> pDimentions)
	{
		switch (renderer::get_current_api())
		{
		case renderer_api::renderer_api::None:
			ARC_CORE_ASSERT(false, "Selected renderer API is not supported (renderer_api::None)");
			return nullptr;
		case renderer_api::renderer_api::OpenGL:
			return create_ref<opengl_texture_2d>(pDimentions);
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;

	}
}