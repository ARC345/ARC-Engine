#pragma once

#include "RendererAPI.h"
#include "ARC\Types\Pointer.h"

namespace arc {
	class render_command
	{
	public:
		inline static void init() {
			sRendererAPI->init();
		};

		inline static void set_viewport(const vec2<uint32_t> bottem_left_coord, const vec2<uint32_t> dimentions) {
			sRendererAPI->set_viewport(bottem_left_coord, dimentions);
		}
		inline static void set_clear_colour(const color4 colour) {
			sRendererAPI->set_clear_colour(colour);
		};
		inline static void clear() {
			sRendererAPI->clear();
		};

		// Draw All if count = 0
		inline static void draw_indexed(const ref<vertex_array>& vertex_array, uint32_t count = 0) {
			sRendererAPI->draw_indexed(vertex_array, count);
		}
		inline static void draw_line(const ref<vertex_array>& vertex_array, uint32_t count = 0) {
			sRendererAPI->draw_line(vertex_array, count);
		}
		inline static void set_line_thickness(float pThickness) {
			sRendererAPI->set_line_thickness(pThickness);
		}

	private:
		static renderer_api* sRendererAPI;
	};
}