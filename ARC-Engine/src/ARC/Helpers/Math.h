#pragma once
#include <cmath>
#include <numeric>
#include "ARC\Core\Log.h"
#include <xtr1common>

#define SM_MATH_FUNC [[nodiscard]] inline constexpr decltype(auto)
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

struct LerpingFunction { template<typename T, typename T1> inline constexpr static T Calculate(T _1, T _2, T1 _Alpha) { return 0; } };

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
	namespace Math {
		template<typename T>
		SM_MATH_FUNC PowIntegral(T _, size_t N) {
			if (N <= 1) return _;
			return Pow<T>(_ * _, N - 1);
		}
		
		template<size_t N, typename T>
		SM_MATH_FUNC DistSqr(const TVec<N, T>& _1, const TVec<N, T>& _2) {
			auto tmp = Math::Sqr(_1 - _2);
			return std::accumulate(tmp.begin(), tmp.end(), 0.f);
		}
		template<size_t N, typename T>
		SM_MATH_FUNC Dist(const TVec<N, T>& _1, const TVec<N, T>& _2)
		{
			return Math::Sqrt(DistSqr(_1, _2));
		}
		
		template<size_t N, typename T>
		SM_MATH_FUNC Dot(const TVec<N, T>& _1, const TVec<N, T>& _2) {
			auto tmp = _1 * _2;
			return std::accumulate(tmp.begin(), tmp.end(), 0);
		}
		template<typename T>
		SM_MATH_FUNC Cross(const TVec3<T>& _1, const TVec3<T>& _2) {
			return (_1.y * _2.z - _2.y * _1.z,
				_1.z * _2.x - _2.z * _1.x,
				_1.x * _2.y - _2.x * _1.y);
		}
		template<size_t N, typename T>
		SM_MATH_FUNC Equal(const TVec<N, T>& _1, const TVec<N, T>& _2, float _Tollerance) { 
			for (size_type i = 0; i < N; i++)
				if(Math::Abs(_1[i] - _2[i]) > _Tollerance)
					return false;
			
			return true;
		}

		template<typename T> SM_MATH_FUNC Sqr(const T& _) { return _*_; }
		template<typename T> SM_MATH_FUNC Sqrt(const T& _) {	return std::sqrtf(_); }
		template<typename T> SM_MATH_FUNC Sin(const T& _) { return std::sin(_); }
		template<typename T> SM_MATH_FUNC Cos(const T& _) { return std::cos(_1); }
		template<typename T> SM_MATH_FUNC Atan2(const T& _1, const T& _2) { return std::atan2(_1, _2); }
		template<typename T> SM_MATH_FUNC Min(T&& t){ return std::forward<T>(t); }
		template<typename T> SM_MATH_FUNC Max(T&& t) { return std::forward<T>(t); }

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

		template<typename T> SM_MATH_FUNC Clamp(T _, T _Min, T _Max) {
			static_assert(std::is_arithmetic<T>::value);
			return Min(Max(_, _Min), _Max) ;
		}
		template<typename T> SM_MATH_FUNC Abs(T _) {
			static_assert(std::is_arithmetic<T>::value);
			return _>=0 ? _ : -_;
		}
		/*
		* @todo Add Curved Interp Support
		*/
		template<typename TLerpingFunction, typename T>
		SM_MATH_FUNC InterpF(T _1, T _2, float _Alpha) {
			return TLerpingFunction::Calculate<T, float>(_1, _2, _Alpha);
		}
		template<typename TLerpingFunction, typename T> 
		SM_MATH_FUNC Interp(T _1, T _2, T _Alpha) {
			return TLerpingFunction::Calculate<T, T>(_1, _2, _Alpha);
		}
		template<typename T>
		SM_MATH_FUNC IsInRange(T _, T _Low, T _High) {
			return (_Low <= _ && _ <= _High);
		}
		template<ERotType From, ERotType To> 
		SM_MATH_FUNC Conv(float _) {
			if constexpr (From == ERotType::Degrees && To == ERotType::Radians) return float(_ * PI / 180.f);
			if constexpr (From == ERotType::Radians && To == ERotType::Degrees) return float(_ / (PI / 180.f));
		}
		template<typename T, ETimeType From, ETimeType To>
		SM_MATH_FUNC Conv(T _Time) {
			if constexpr (From == To) return _Time;
			if constexpr (To == ETimeType::Day)	return Conv<T, From, ETimeType::Hour>(_Time) / 24.f;
			if constexpr (From == ETimeType::Day)	return Conv<T, ETimeType::Hour, To>(_Time * 24.f);

			auto FromI = int8_t(From);
			auto ToI = int8_t(To);
			
			if (FromI >= 1) {
				if (ToI >= 10)	return _Time * pow(T(10.f), ToI - FromI);// convert to seconds
				else if(ToI <= 0) return Conv<T, From, ETimeType::Seconds>(_Time) / pow(T(60.f), 0 - ToI);
			}

			return Conv<T, ETimeType::Seconds, To>(_Time * pow(T(60.f), 0 - FromI));
		}

		SM_MATH_FUNC Radians(float _) { return Conv<ERotType::Degrees, ERotType::Radians>(_); }
		SM_MATH_FUNC Degrees(float _) { return Conv<ERotType::Radians, ERotType::Degrees>(_); }
	};
}