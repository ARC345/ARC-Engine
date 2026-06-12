#pragma once
#include "ARC\Types\Pointer.h"
#include "ARC\Types\vector.h"
#include "glm\ext\vector_float2.hpp"

namespace arc { class texture_2d; }

namespace arc {
	class sub_texture_2d
	{
	public:
		sub_texture_2d(const ref<texture_2d>& texture, const vec2& min, const vec2& max);

		inline const ref<texture_2d> get_texture() const { return texture_; };
		inline const glm::vec2* get_tex_coords() const { return tex_coords_; };

		static ref<sub_texture_2d> CreateFromCoords(const ref<texture_2d>& texture, const vec2& coords, const vec2& cell_size, const vec2& sprite_size={1, 1});
	protected:
	private:
		ref<texture_2d> texture_;
		glm::vec2 tex_coords_[4];
	};
}