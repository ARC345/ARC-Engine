#pragma once
#include "Utils\Utils.hpp"
#include "Utils\MPL\Interface.hpp"
#include <limits>

namespace ARC::ECS {

	ECS_STRONG_TYPEDEF(std::size_t, DataIndex_t);
	ECS_STRONG_TYPEDEF(std::size_t, EntityIndex_t);

	ECS_STRONG_TYPEDEF(std::size_t, HandleDataIndex_t);
	ECS_STRONG_TYPEDEF(int, Counter_t);

	inline constexpr size_t InvalidEntityID = std::numeric_limits<size_t>::max();
	inline constexpr size_t DeadEntityID = std::numeric_limits<size_t>::max()-1;
	inline constexpr size_t MaxEntityID = std::numeric_limits<size_t>::max()-10; // 10 ids reserved for future use
}