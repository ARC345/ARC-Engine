#pragma once
#include "Object.h"
#include"glm\glm.hpp"

namespace ARC {
	class CPrimitive2D :
		public CObject
	{
		public:
			glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
			glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
			float Rotation = 0.0f;
	};
}