#pragma once
#include <cmath>
#include <numeric>

#define PI 3.14159265358979323
#define PIf 3.14159265358979323f

enum class rot_type : uint8_t{
	Degrees,
	Radians
};
enum class time_type : int8_t{
	Day = -10,
	Hour = -2,
	Minute = -1,
	Seconds = 10,
	Milli = 13,
	Micro = 16,
	Nano = 19
};

namespace arc { struct math; }

struct lerping_function { template<typename T, typename T1> inline constexpr static T calculate(T _1, T _2, T1 alpha) { return 0; } };

struct Linear : lerping_function { 
	template<typename T, typename T1>
	inline constexpr static
	T calculate(T _1, T _2, T1 alpha) { 
		return _1 + (_2 - _1) * alpha; 
	} 
};
	
template<typename T0>
struct Flip : lerping_function {
	template<typename T, typename T1>
	inline constexpr static
	T calculate(T _1, T _2, T1 alpha) {
		return T0::calculate(_1, _2, 1 - alpha);
	}
};
template<size_t Exponent>
struct ease_in : lerping_function {
	template<typename T, typename T1>
	inline constexpr static
	T calculate(T _1, T _2, T1 alpha) {
		return Linear::calculate(_1, _2, arc::math::Pow(alpha, Exponent));
	}
};
template<size_t Exponent>
struct ease_in_out : lerping_function {
	template<typename T, typename T1>
	inline constexpr static
	T calculate(T _1, T _2, T1 alpha) {
		return Linear::calculate(_1, _2, Linear::calculate(
			ease_in<Exponent>::calculate(_1, _2, alpha),
			Flip<ease_in<Exponent>>::calculate(_1, _2, alpha),
			alpha
		));
	}
};

template<size_t Exponent>
using EaseOut = Flip<ease_in<Exponent>>;

using Quadratic = ease_in<2>;
using Cubic = ease_in<3>;

namespace arc {
	struct math {
		template<typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) Dot(const T& _1, const T& _2) {
			auto tmp = _1 * _2;
			return std::accumulate(tmp.begin(), tmp.end(), T::value_type(0.0f));
		}
		
		template<typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) Sqr(const T& _) { return _*_; }

		template<typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) Sqrt(const T& _) { return std::sqrtf(_); }

		template<typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) Sin(const T& _) { return std::sin(_); }

		template<typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) Cos(const T& _) { return std::cos(_); }

		template<typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) Atan2(const T& _1, const T& _2) { return std::atan2(_1, _2); }

		template<typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) Min(T&& t) { return std::forward<T>(t); }
		
		template<typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) Max(T&& t) { return std::forward<T>(t); }

		template<typename T, typename TT>
		[[nodiscard]] inline constexpr static
		decltype(auto) Pow(T _, TT __) {
			return pow(_, __);
		}

		template<typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) DistSqr(const T& _1, const T& _2) {
			auto tmp = math::Sqr(_1 - _2);
			return std::accumulate(tmp.begin(), tmp.end(), 0.f);
		}
		template<typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) Dist(const T& _1, const T& _2)
		{
			return math::Sqrt(DistSqr(_1, _2));
		}

		template<typename T0, typename T1, typename... Ts>
		[[nodiscard]] inline constexpr static
		typename std::common_type<T0, T1, Ts...>::type Min(T0&& val1, T1&& val2, Ts&&... vs)
		{
			if (val1 < val2)
				return Min(val1, std::forward<Ts>(vs)...);
			else
				return Min(val2, std::forward<Ts>(vs)...);
		}
		

		template<typename T0, typename T1, typename... Ts>
		[[nodiscard]] inline constexpr static
		typename std::common_type<T0, T1, Ts...>::type Max(T0&& val1, T1&& val2, Ts&&... vs)
		{
			if (val1 > val2)
				return Max(val1, std::forward<Ts>(vs)...);
			else
				return Max(val2, std::forward<Ts>(vs)...);
		}

		template<typename T>
		[[nodiscard]] inline constexpr static
		auto Mean(const T& tmp)
		{
			return std::accumulate(tmp.begin(), tmp.end(), T::value_type(0)) / std::size(tmp);
		}

		template<typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) Clamp(T _, T min, T max) {
			static_assert(std::is_arithmetic_v<T>);
			return Min(Max(_, min), max) ;
		}
		template<typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) Abs(T _) {
			static_assert(std::is_arithmetic_v<T>);
			return _>=0 ? _ : -_;
		}
		template<typename T>
			[[nodiscard]] inline constexpr static
			decltype(auto) is_equal(const T& _1, const T& _2, T tollerance) {
			return math::Abs(_1 - _2) <= tollerance;
		}
		/*
		* @todo Add Curved Interp Support
		*/
		template<typename TLerpingFunction, typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) InterpF(T _1, T _2, float alpha) {
			return TLerpingFunction::template calculate<T, float>(_1, _2, alpha);
		}
		template<typename TLerpingFunction, typename T> 
		[[nodiscard]] inline constexpr static
		decltype(auto) Interp(T _1, T _2, T alpha) {
			return TLerpingFunction::template calculate<T, T>(_1, _2, alpha);
		}
		template<typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) is_in_range(T _, T low, T high) {
			return (low <= _ && _ <= high);
		}
		template<rot_type From, rot_type To> 
		[[nodiscard]] inline constexpr static
		decltype(auto) Conv(float _) {
			
			if constexpr (From == rot_type::Degrees && To == rot_type::Radians) { 
				_ = (float)fmod(_, 360.f);
				if (_ < 0)
					_ += 360.f;
				return float(_ * PIf / 180.f);
			}
			if constexpr (From == rot_type::Radians && To == rot_type::Degrees){  
				_ = (float)fmod(_, PIf*2.f);
				if (_ < 0.f)
					_ += PIf * 2.f;
				return float(_ / (PIf / 180.f)); 
			}
		}
		template<typename T, time_type From, time_type To>
		[[nodiscard]] inline constexpr static
		decltype(auto) Conv(T time) {
			if constexpr (From == To) return time;
			if constexpr (To == time_type::Day)	return Conv<T, From, time_type::Hour>(time) / 24.f;
			if constexpr (From == time_type::Day)	return Conv<T, time_type::Hour, To>(time * 24.f);

			auto FromI = int8_t(From);
			auto ToI = int8_t(To);
			
			if (FromI >= 1) {
				if (ToI >= 10)	return time * Pow(T(10.f), ToI - FromI);// convert to seconds
				else if(ToI <= 0) return Conv<T, From, time_type::Seconds>(time) / Pow(T(60.f), 0 - ToI);
			}

			return Conv<T, time_type::Seconds, To>(time * Pow(T(60.f), 0 - FromI));
		}

		[[nodiscard]] inline constexpr static 
		decltype(auto) Radians(float _) { return Conv<rot_type::Degrees, rot_type::Radians>(_); }
		[[nodiscard]] inline constexpr static 
		decltype(auto) Degrees(float _) { return Conv<rot_type::Radians, rot_type::Degrees>(_); }
	};
}