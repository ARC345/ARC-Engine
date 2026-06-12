#pragma once
#include "Object.h"
#include "ARC/Types/Color.h"
#include "ARC/Types/Vector.h"
#include "ARC/Types/Transform2D.h"
#include "ARC/Types/Pointer.h"

namespace arc { class sub_texture_2d; }
namespace arc { class texture_2d; }

namespace arc {
	class primitive_2d :
		public object
	{
	public:
		inline const vec3& get_location() const { return Transform.Location; }
		inline vec3 get_location() { return Transform.Location; }

		inline const float& get_rotation() const { return Transform.Rotation; }
		inline float get_rotation() { return Transform.Rotation; }

		inline const vec2& get_scale() const { return Transform.Scale; }
		inline vec2 get_scale() { return Transform.Scale; }

		inline const transform_2d& get_transform() const { return Transform; }
		inline transform_2d get_transform() { return Transform; }

	public:
		transform_2d Transform;
		transparency_type TransparencyLevel;
		color4 Color = color4::White();
		ref<texture_2d> Texture = nullptr;
		vec2 TextureScaling = vec2::OneVector();
	};
}