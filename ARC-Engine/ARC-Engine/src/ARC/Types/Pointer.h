#pragma once
#include <memory>

namespace ARC {
	template<typename T>
	using TRef = std::shared_ptr<T>;
	
	template<typename T>
	using TScope = std::unique_ptr<T>;
}