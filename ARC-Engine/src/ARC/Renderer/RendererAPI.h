#pragma once

#include "..\..\..\out\glm\glm\glm.hpp"
#include "ARC\Types\Pointer.h"
#include "ARC\Types\Color.h"

namespace arc { class vertex_array; }

namespace arc {
	class renderer_api {
	public:
		enum class renderer_api
		{
			None = 0,
			OpenGL = 1
		};
	public:
		virtual ~renderer_api() = default;

		virtual void init() = 0;
		virtual void set_viewport(const vec2<uint32_t> bottem_left_coord, const vec2<uint32_t> dimentions) = 0;
		virtual void set_clear_colour(const color4 colour) = 0;
		virtual void clear() = 0;

		virtual void draw_indexed(const ref<vertex_array>& vertex_array, uint32_t count) = 0;
		virtual void draw_line(const ref<vertex_array>& vertex_array, uint32_t count) = 0;
		virtual void set_line_thickness(float pThickness) = 0;

		inline static renderer_api get_api() { return api; };
	private:
		inline static renderer_api api = renderer_api::OpenGL;
	};
}