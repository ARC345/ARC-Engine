#pragma once
#include "Object.h"

#include"..\..\..\out\glm\glm\glm.hpp"

namespace ARC {
	class CPrimitive2D :
		public CObject
	{
		public:
			glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
			glm::vec3 Scale = { 0.0f, 0.0f, 0.0f };
			float Rotation = 0.0f;
	};
}