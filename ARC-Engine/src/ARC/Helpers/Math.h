#pragma once
#include <cmath>
#include "..\Core\Log.h"

#define SM_MATH_FUNC [[nodiscard]] inline constexpr
#define PI 3.14159265358979323

enum ERotType : uint8_t{
	Degree,
	Radians
};
enum ETimeType : int8_t{
	Day = -10,
	Hour = -2,
	Minute = -1,
	Seconds = 10,
	Milli = 13,
	Micro = 16,
	Nano = 19
};

namespace ARC {
	template<typename T> inline constexpr T Pow(T _, size_t N) {
		if (N<=1) return _;
		return Pow<T>(_*_, N-1);
	}
}
struct LerpingFunction { template<typename T, typename T1> inline constexpr static T Calculate(T _1, T _2, T1 _Alpha) { return 0} };

struct Linear : LerpingFunction { 
	template<typename T, typename T1> inline constexpr static T Calculate(T _1, T _2, T1 _Alpha) { 
		return _1 + (_2 - _1) * _Alpha; 
	} 
};
	
template<typename T0>
struct Flip : LerpingFunction {
	template<typename T, typename T1> inline constexpr static T Calculate(T _1, T _2, T1 _Alpha) {
		return T0::Calculate(_1, _2, 1 - _Alpha);
	}
};
template<size_t Exponent>
struct EaseIn : LerpingFunction {
	template<typename T, typename T1> inline constexpr static T Calculate(T _1, T _2, T1 _Alpha) {
		return Linear::Calculate(_1, _2, ARC::Pow(_Alpha, Exponent));
	}
};
template<size_t Exponent>
struct EaseInOut : LerpingFunction {
	template<typename T, typename T1> inline constexpr static T Calculate(T _1, T _2, T1 _Alpha) {
		return Linear::Calculate(_1, _2, Linear::Calculate(
			EaseIn::Calculate(_1, _2, _Alpha),
			Flip<EaseIn>::Calculate(_1, _2, _Alpha),
			_Alpha
		));
	}
};

template<size_t Exponent>
using EaseOut = Flip<EaseIn<Exponent>>;

using Quadratic = EaseIn<2>;
using Cubic = EaseIn<3>;

namespace ARC {
	namespace Math {
		template<typename T> SM_MATH_FUNC T Sqr(T _1) {
			static_assert(std::is_arithmetic<T>::value);
			return _1*_1;
		}
		template<typename T> SM_MATH_FUNC T Sqrt(T _1) {
			static_assert(std::is_arithmetic<T>::value);
			return std::sqrtf(_1);
		}
		template<typename T> SM_MATH_FUNC T Sin(T _1) {
			static_assert(std::is_arithmetic<T>::value);
			return std::sin(_1);
		}
		template<typename T> SM_MATH_FUNC T Cos(T _1) {
			static_assert(std::is_arithmetic<T>::value);
			return std::cos(_1);
		}
		template<typename T>
		T Min(T&& t)
		{
			return std::forward<T>(t);
		}

		template<typename T0, typename T1, typename... Ts>
		typename std::common_type<T0, T1, Ts...>::type Min(T0&& val1, T1&& val2, Ts&&... vs)
		{
			if (val1 < val2)
				return Min(val1, std::forward<Ts>(vs)...);
			else
				return Min(val2, std::forward<Ts>(vs)...);
		}
		
		template<typename T>
		T Max(T&& t)
		{
			return std::forward<T>(t);
		}

		template<typename T0, typename T1, typename... Ts>
		typename std::common_type<T0, T1, Ts...>::type Max(T0&& val1, T1&& val2, Ts&&... vs)
		{
			if (val1 > val2)
				return Min(val1, std::forward<Ts>(vs)...);
			else
				return Min(val2, std::forward<Ts>(vs)...);
		}

		template<typename T> SM_MATH_FUNC T Clamp(T _1, T _Min, T _Max) {
			static_assert(std::is_arithmetic<T>::value);
			return Min(Max(_1, _Min), _Max) ;
		}
		template<typename T> SM_MATH_FUNC T Abs(T _1) {
			static_assert(std::is_arithmetic<T>::value);
			return _1>=0 ? _1 : -_1;
		}
		/*
		* @todo Add Curved Lerp Support
		*/
		template<typename TLerpingFunction, typename T> SM_MATH_FUNC T InterpF(T _1, T _2, float _Alpha) {
			return TLerpingFunction::Calculate<T, float>(_1, _2, _Alpha);
		}
		template<typename TLerpingFunction, typename T> SM_MATH_FUNC T Interp(T _1, T _2, T _Alpha) {
			return TLerpingFunction::Calculate<T, T>(_1, _2, _Alpha);
		}
		template<typename T> SM_MATH_FUNC bool IsInRange(T _1, T _Low, T _High) {
			return (_Low <= _1 && _1 <= _High);
		}
		template<ERotType From, ERotType To> SM_MATH_FUNC float Conv(float _1) {
			if (From == ERotType::Degree && To == ERotType::Radians) return float(_1 * PI / 180.f);
			if (From == ERotType::Radians && To == ERotType::Degree) return float(_1 / (PI / 180.f));
		}
		template<typename T, ETimeType From, ETimeType To> SM_MATH_FUNC T Conv(T _Time) {
			if (From == To) return _Time;
			if (To == ETimeType::Day)	return Conv<T, From, Hour>(_Time) / 24.f;
			if (From == ETimeType::Day)	return Conv<T, Hour, To>(_Time * 24.f);

			int8_t FromI = int8_t(From);
			int8_t ToI = int8_t(To);
			
			if (FromI >= 1) {
				if (ToI >= 10)	return _Time * pow(T(10.f), ToI - FromI);// convert to seconds
				else if(ToI <= 0) return Conv<T, From, Seconds>(_Time) / pow(T(60.f), 0 - ToI);
			}

			return Conv<T, Seconds, To>(_Time * pow(T(60.f), 0 - FromI));
		}
	};
}