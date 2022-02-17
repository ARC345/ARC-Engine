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

struct LerpingFunction { template<typename T, typename T1> inline constexpr static T Calculate(T _1, T _2, T1 _Alpha) { return 0} };
struct Linear : LerpingFunction { 
	template<typename T, typename T1> inline constexpr static T Calculate(T _1, T _2, T1 _Alpha) { 
		return _1 + (_2 - _1) * _Alpha; } 
	};

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
		template<typename T> SM_MATH_FUNC T Min(T _1, T _2) {
			static_assert(std::is_arithmetic<T>::value);
			return _1 < _2 ? _1 : _2;
		}
		template<typename T> SM_MATH_FUNC T Max(T _1, T _2) {
			static_assert(std::is_arithmetic<T>::value);
			return _1 > _2 ? _1 : _2;
		}
		template<typename T> SM_MATH_FUNC T Abs(T _1) {
			static_assert(std::is_arithmetic<T>::value);
			return _1>=0 ? _1 : -_1;
		}
		/*
		* @todo Add Curved Lerp Support
		*/
		template<typename T, typename TLerpingFunction> SM_MATH_FUNC T LerpF(T _1, T _2, float _Alpha) {
			return TLerpingFunction::Calculate<T, float>(_1, _2, _Alpha);
		}
		template<typename T, typename TLerpingFunction> SM_MATH_FUNC T Lerp(T _1, T _2, T _Alpha) {
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