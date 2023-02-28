#pragma once

#include "glm/glm.hpp"
#include "ARC/Helpers/Conversions.h"
#include "ARC/Types/Transform2D.h"
#include "ARC/Types/Transform3D.h"

namespace ARC
{
	using FGLMMat4 = glm::mat4;
	using FGLMMat3 = glm::mat3;
	using FGLMMat2 = glm::mat2;
	
	using FGLMVec4 = glm::vec4;
	using FGLMVec3 = glm::vec3;
	using FGLMVec2 = glm::vec2;

	template <>
	struct SConvert<FGLMMat4, FTransform2D>
	{
		static FGLMMat4 Conv(const FTransform2D& p);
	};
	template <>
	struct SConvert<FTransform3D, FGLMMat4>
	{
		static bool CanConv(const FGLMMat4& p);
		static FTransform3D Conv(const FGLMMat4& p);
	};
	template <>
	struct SConvert<FTransform2D, FGLMMat4>
	{
		static FTransform2D Conv(const FGLMMat4& p);
	};
}