#pragma once

#include "glm/glm.hpp"
#include "ARC/Helpers/Conversions.h"
#include "ARC/Types/Transform2D.h"

namespace ARC
{
	using FGMat4 = glm::mat4;
	using FGMat3 = glm::mat3;
	using FGMat2 = glm::mat2;
	
	using FGVec4 = glm::vec4;
	using FGVec3 = glm::vec3;
	using FGVec2 = glm::vec2;

	template <>
	struct SConvert<FGMat4, FTransform2D>
	{
		static FGMat4 Conv(const FTransform2D& p);
	};
}