#include "arc_pch.h"
#include "SubTexture2D.h"
#include "ARC\Renderer\Texture.h"

namespace arc {

	sub_texture_2d::sub_texture_2d(const ref<texture_2d>& texture, const vec2& min, const vec2& max) :
		texture_(texture)
	{
		tex_coords_[0] = { min.x, min.y };
		tex_coords_[1] = { max.x, min.y };
		tex_coords_[2] = { max.x, max.y };
		tex_coords_[3] = { min.x, max.y };
	}

	ref<sub_texture_2d> sub_texture_2d::CreateFromCoords(const ref<texture_2d>& texture, const vec2& coords, const vec2& cell_size, const vec2& sprite_size)
	{
		return create_ref<sub_texture_2d>(
			texture,
			vec2(coords.x * cell_size.x / texture->Dimensions.x, coords.y * cell_size.y / texture->Dimensions.y),
			vec2((coords.x + sprite_size.x) * cell_size.x / texture->Dimensions.x, (coords.y + sprite_size.y) * cell_size.y / texture->Dimensions.y)
			);
	}

}