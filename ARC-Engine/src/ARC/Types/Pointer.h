#pragma once
#include <memory>

namespace arc {
	template<typename T>
	using ref = std::shared_ptr<T>;
	
	template<typename T>
	using TScope = std::unique_ptr<T>;

	template<typename T, typename ... TArgs>
	inline constexpr ref<T> create_ref(TArgs&& ... args) {
		return std::make_shared<T>(std::forward<TArgs>(args)...);
	}
	template<typename T, typename ... TArgs>
	inline constexpr TScope<T> create_scope(TArgs&& ... args) {
		return std::make_unique<T>(std::forward<TArgs>(args)...);
	}
}