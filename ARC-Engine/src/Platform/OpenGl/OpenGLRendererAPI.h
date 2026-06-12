#pragma once
#include "ARC\Renderer\RendererAPI.h"

namespace arc {
	class opengl_renderer_api : public renderer_api
	{
	public:
		virtual void init() override;
		virtual void set_viewport(const vec2<uint32_t> bottem_left_coord, const vec2<uint32_t> dimentions) override;
		virtual void set_clear_colour(const color4 colour) override;
		virtual void clear() override;
		virtual void draw_indexed(const ref<vertex_array>& vertex_array, uint32_t count) override;
		virtual void draw_line(const ref<vertex_array>& vertex_array, uint32_t count) override;
		virtual void set_line_thickness(float pThickness) override;
	};
}