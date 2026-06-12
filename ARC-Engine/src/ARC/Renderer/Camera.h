#pragma once

#include "ARC/Wrappers/Glm.h"

namespace arc
{
	class camera
	{
	public:
		camera() = default;
		camera(const mat4& pProjection) : projection_(pProjection){};

		const mat4& get_projection() const { return projection_; }

	protected:
		mat4 projection_ = mat4(1.0f);
	};
}