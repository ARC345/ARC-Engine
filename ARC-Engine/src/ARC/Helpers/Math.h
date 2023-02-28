#pragma once
#include <cmath>
#include <numeric>

#define PI 3.14159265358979323

enum class ERotType : uint8_t{
	Degrees,
	Radians
};
enum class ETimeType : int8_t{
	Day = -10,
	Hour = -2,
	Minute = -1,
	Seconds = 10,
	Milli = 13,
	Micro = 16,
	Nano = 19
};

namespace ARC { struct SMath; }

struct LerpingFunction { template<typename T, typename T1> inline constexpr static T Calculate(T _1, T _2, T1 _Alpha) { return 0; } };

struct Linear : LerpingFunction { 
	template<typename T, typename T1>
	inline constexpr static
	T Calculate(T _1, T _2, T1 _Alpha) { 
		return _1 + (_2 - _1) * _Alpha; 
	} 
};
	
template<typename T0>
struct Flip : LerpingFunction {
	template<typename T, typename T1>
	inline constexpr static
	T Calculate(T _1, T _2, T1 _Alpha) {
		return T0::Calculate(_1, _2, 1 - _Alpha);
	}
};
template<size_t Exponent>
struct EaseIn : LerpingFunction {
	template<typename T, typename T1>
	inline constexpr static
	T Calculate(T _1, T _2, T1 _Alpha) {
		return Linear::Calculate(_1, _2, ARC::SMath::Pow(_Alpha, Exponent));
	}
};
template<size_t Exponent>
struct EaseInOut : LerpingFunction {
	template<typename T, typename T1>
	inline constexpr static
	T Calculate(T _1, T _2, T1 _Alpha) {
		return Linear::Calculate(_1, _2, Linear::Calculate(
			EaseIn<Exponent>::Calculate(_1, _2, _Alpha),
			Flip<EaseIn<Exponent>>::Calculate(_1, _2, _Alpha),
			_Alpha
		));
	}
};

template<size_t Exponent>
using EaseOut = Flip<EaseIn<Exponent>>;

using Quadratic = EaseIn<2>;
using Cubic = EaseIn<3>;

namespace ARC {
	struct SMath {
		template<typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) Dot(const T& _1, const T& _2) {
			auto tmp = _1 * _2;
			return std::accumulate(tmp.begin(), tmp.end(), 0);
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
			auto tmp = SMath::Sqr(_1 - _2);
			return std::accumulate(tmp.begin(), tmp.end(), 0.f);
		}
		template<typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) Dist(const T& _1, const T& _2)
		{
			return SMath::Sqrt(DistSqr(_1, _2));
		}

		template<typename T0, typename T1, typename... Ts>
		typename std::common_type<T0, T1, Ts...>::type Min(T0&& val1, T1&& val2, Ts&&... vs)
		{
			if (val1 < val2)
				return Min(val1, std::forward<Ts>(vs)...);
			else
				return Min(val2, std::forward<Ts>(vs)...);
		}
		

		template<typename T0, typename T1, typename... Ts>
		typename std::common_type<T0, T1, Ts...>::type Max(T0&& val1, T1&& val2, Ts&&... vs)
		{
			if (val1 > val2)
				return Max(val1, std::forward<Ts>(vs)...);
			else
				return Max(val2, std::forward<Ts>(vs)...);
		}

		template<typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) Clamp(T _, T _Min, T _Max) {
			static_assert(std::is_arithmetic_v<T>);
			return Min(Max(_, _Min), _Max) ;
		}
		template<typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) Abs(T _) {
			static_assert(std::is_arithmetic_v<T>);
			return _>=0 ? _ : -_;
		}
		template<typename T>
			[[nodiscard]] inline constexpr static
			decltype(auto) IsEqual(const T& _1, const T& _2, T _Tollerance) {
			return SMath::Abs(_1 - _2) <= _Tollerance;
		}
		/*
		* @todo Add Curved Interp Support
		*/
		template<typename TLerpingFunction, typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) InterpF(T _1, T _2, float _Alpha) {
			return TLerpingFunction::template Calculate<T, float>(_1, _2, _Alpha);
		}
		template<typename TLerpingFunction, typename T> 
		[[nodiscard]] inline constexpr static
		decltype(auto) Interp(T _1, T _2, T _Alpha) {
			return TLerpingFunction::template Calculate<T, T>(_1, _2, _Alpha);
		}
		template<typename T>
		[[nodiscard]] inline constexpr static
		decltype(auto) IsInRange(T _, T _Low, T _High) {
			return (_Low <= _ && _ <= _High);
		}
		template<ERotType From, ERotType To> 
		[[nodiscard]] inline constexpr static
		decltype(auto) Conv(float _) {
			if constexpr (From == ERotType::Degrees && To == ERotType::Radians) return float(_ * PI / 180.f);
			if constexpr (From == ERotType::Radians && To == ERotType::Degrees) return float(_ / (PI / 180.f));
		}
		template<typename T, ETimeType From, ETimeType To>
		[[nodiscard]] inline constexpr static
		decltype(auto) Conv(T _Time) {
			if constexpr (From == To) return _Time;
			if constexpr (To == ETimeType::Day)	return Conv<T, From, ETimeType::Hour>(_Time) / 24.f;
			if constexpr (From == ETimeType::Day)	return Conv<T, ETimeType::Hour, To>(_Time * 24.f);

			auto FromI = int8_t(From);
			auto ToI = int8_t(To);
			
			if (FromI >= 1) {
				if (ToI >= 10)	return _Time * Pow(T(10.f), ToI - FromI);// convert to seconds
				else if(ToI <= 0) return Conv<T, From, ETimeType::Seconds>(_Time) / Pow(T(60.f), 0 - ToI);
			}

			return Conv<T, ETimeType::Seconds, To>(_Time * Pow(T(60.f), 0 - FromI));
		}

		[[nodiscard]] inline constexpr static 
		decltype(auto) Radians(float _) { return Conv<ERotType::Degrees, ERotType::Radians>(_); }
		[[nodiscard]] inline constexpr static 
		decltype(auto) Degrees(float _) { return Conv<ERotType::Radians, ERotType::Degrees>(_); }
	};
}