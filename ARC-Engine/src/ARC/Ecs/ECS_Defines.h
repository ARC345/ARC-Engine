#pragma once
#include "Utils\Utils.hpp"
#include "Utils\MPL\Interface.hpp"

namespace ARC::ECS {

	ECS_STRONG_TYPEDEF(std::size_t, DataIndex_t);
	ECS_STRONG_TYPEDEF(std::size_t, EntityIndex_t);

	ECS_STRONG_TYPEDEF(std::size_t, HandleDataIndex_t);
	ECS_STRONG_TYPEDEF(int, Counter_t);
}