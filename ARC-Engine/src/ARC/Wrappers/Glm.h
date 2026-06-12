#pragma once

#include "glm/glm.hpp"
#include "ARC/Helpers/Conversions.h"
#include "ARC/Types/Transform2D.h"
#include "ARC/Types/Transform3D.h"

namespace arc
{
	using mat4 = glm::mat4;
	using mat3 = glm::mat3;
	using mat2 = glm::mat2;
	
	using vec4 = glm::vec4;
	using vec3 = glm::vec3;
	using vec2 = glm::vec2;

	template <>
	struct convert<mat4, transform_2d>
	{
		static mat4 Conv(const transform_2d& p);
	};
	template <>
	struct convert<transform_3d, mat4>
	{
		static bool CanConv(const mat4& p);
		static transform_3d Conv(const mat4& p);
	};
	template <>
	struct convert<transform_2d, mat4>
	{
		static transform_2d Conv(const mat4& p);
	};
}