#pragma once

#include "../../../out/glm/glm/glm.hpp"

namespace ARC {
	class CColor
	{
	public:
		constexpr static glm::vec4 Red = { 1.0f, 0.0f, 0.0f, 1.0f};
		constexpr static glm::vec4 Green = { 0.0f, 1.0f, 0.0f, 1.0f};
		constexpr static glm::vec4 Blue = { 0.0f, 0.0f, 1.0f, 1.0f};
	};
}