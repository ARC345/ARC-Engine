#pragma once

#define SM_MATH_FUNC [[nodiscard]] inline constexpr

namespace ARC {
	namespace Math {
		template<typename T> SM_MATH_FUNC T Sqr(T _1) {
			static_assert(std::is_arithmetic<T::value);
			return _1*_1;
		}
		template<typename T> SM_MATH_FUNC T Sqrt(T _1) {
			static_assert(std::is_arithmetic<T::value);
			return std::sqrt<T>(_1);
		}
	};
}