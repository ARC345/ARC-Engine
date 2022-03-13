#pragma once
#include <memory>

namespace ARC {
	template<typename T>
	using TRef = std::shared_ptr<T>;
	
	template<typename T>
	using TScope = std::unique_ptr<T>;

	template<typename T, typename ... TArgs>
	inline constexpr TRef<T> CreateRef(TArgs&& ... _Args) {
		return std::make_shared<T>(std::forward<TArgs>(_Args)...);
	}
	template<typename T, typename ... TArgs>
	inline constexpr TScope<T> CreateScope(TArgs&& ... _Args) {
		return std::make_unique<T>(std::forward<TArgs>(_Args)...);
	}
}