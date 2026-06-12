#pragma once
#include "RenderCommand.h"
#include "RendererAPI.h"

#include "ARC\Wrappers\Glm.h"

namespace arc { class shader; }
namespace arc { class vertex_array; }
namespace arc { class orthographic_camera; }

namespace arc {
	class renderer {
	public:
		static void init();
		static void on_window_resize(vec2<uint32_t> dimentions);
		static void begin_scene(orthographic_camera& cam);
		static void end_scene();

		static mat4 get_view_projection_matrix() { return scene_data_->ViewProjectionMatrix; }

		static void Submit(const ref<shader>& shader, const ref<vertex_array>& vertex_array, const mat4& transform = mat4(1.f));

		inline static renderer_api::renderer_api get_current_api() { return renderer_api::get_api(); }

	private:
		struct scene_data
		{
			mat4 ViewProjectionMatrix;
		};

		static scene_data* scene_data_;
	};	
}