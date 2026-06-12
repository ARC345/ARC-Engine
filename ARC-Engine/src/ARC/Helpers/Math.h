#pragma once
#include <cmath>
#include <numeric>
#include <type_traits>

#define PI 3.14159265358979323
#define PIf 3.14159265358979323f

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

// Forward declare glm types for SFINAE detection
namespace glm { class vec2; class vec3; class vec4; }

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
		// --- Dot product: GLM-backed for float vectors, generic for integral ---
	private:
		template<typename T, typename = void>
		struct IsGLMConvertible : std::false_type {};
		template<typename T>
		struct IsGLMConvertible<T, std::void_t<decltype(glm::dot(std::declval<const T&>(), std::declval<const T&>()))>> : std::true_type {};

	public:
		template<typename T>
		[[nodiscard]] inline static
		std::enable_if_t<IsGLMConvertible<T>::value, decltype(glm::dot(std::declval<const T&>(), std::declval<const T&>()))>
		Dot(const T& _1, const T& _2) {
			return glm::dot(_1, _2);
		}

		template<typename T>
		[[nodiscard]] inline static
		std::enable_if_t<!IsGLMConvertible<T>::value, typename T::value_type>
		Dot(const T& _1, const T& _2) {
			auto tmp = _1 * _2;
			return std::accumulate(tmp.begin(), tmp.end(), typename T::value_type(0));
		}

		// --- Distance: GLM for float, generic for integral ---
	private:
		template<typename T, typename = void>
		struct HasGLMDistance : std::false_type {};
		template<typename T>
		struct HasGLMDistance<T, std::void_t<decltype(glm::distance(std::declval<const T&>(), std::declval<const T&>()))>> : std::true_type {};

	public:
		template<typename T>
		[[nodiscard]] inline static
		std::enable_if_t<HasGLMDistance<T>::value, decltype(glm::distance(std::declval<const T&>(), std::declval<const T&>()))>
		Dist(const T& _1, const T& _2) {
			return glm::distance(_1, _2);
		}

		template<typename T>
		[[nodiscard]] inline static
		std::enable_if_t<!HasGLMDistance<T>::value, typename T::type_float>
		Dist(const T& _1, const T& _2) {
			return _1.Dist(_2);
		}

		template<typename T>
		[[nodiscard]] inline static
		std::enable_if_t<HasGLMDistance<T>::value, decltype(glm::distance2(std::declval<const T&>(), std::declval<const T&>()))>
		DistSqr(const T& _1, const T& _2) {
			return glm::distance2(_1, _2);
		}

		template<typename T>
		[[nodiscard]] inline static
		std::enable_if_t<!HasGLMDistance<T>::value, typename T::type_float>
		DistSqr(const T& _1, const T& _2) {
			return _1.DistSqr(_2);
		}

		// --- Length ---
		template<typename T>
		[[nodiscard]] inline static
		std::enable_if_t<HasGLMDistance<T>::value, decltype(glm::length(std::declval<const T&>()))>
		Length(const T& _) {
			return glm::length(_);
		}

		template<typename T>
		[[nodiscard]] inline static
		std::enable_if_t<!HasGLMDistance<T>::value, typename T::type_float>
		Length(const T& _) {
			return _.Length();
		}

		// --- Normalize ---
		template<typename T>
		[[nodiscard]] inline static
		std::enable_if_t<HasGLMDistance<T>::value, T>
		Normalize(const T& _) {
			return T(glm::normalize(_));
		}

		template<typename T>
		[[nodiscard]] inline static
		std::enable_if_t<!HasGLMDistance<T>::value, T>
		Normalize(const T& _) {
			return _.Normalize();
		}

		// --- Cross product ---
		template<typename T>
		[[nodiscard]] inline static
		std::enable_if_t<HasGLMDistance<T>::value, T>
		Cross(const T& _1, const T& _2) {
			return T(glm::cross(_1, _2));
		}

		template<typename T>
		[[nodiscard]] inline static
		std::enable_if_t<!HasGLMDistance<T>::value, T>
		Cross(const T& _1, const T& _2) {
			return T().Cross(_1, _2);
		}

		// --- Scalar math ---
		template<typename T>
		[[nodiscard]] inline static
		auto Sqr(const T& _) { return _ * _; }

		template<typename T>
		[[nodiscard]] inline static
		auto Sqrt(const T& _) { return std::sqrt(_); }

		template<typename T>
		[[nodiscard]] inline static
		auto Sin(const T& _) { return std::sin(_); }

		template<typename T>
		[[nodiscard]] inline static
		auto Cos(const T& _) { return std::cos(_); }

		template<typename T>
		[[nodiscard]] inline static
		auto Atan2(const T& _1, const T& _2) { return std::atan2(_1, _2); }

		template<typename T>
		[[nodiscard]] inline static
		auto Min(T&& t) { return std::forward<T>(t); }
		
		template<typename T>
		[[nodiscard]] inline static
		auto Max(T&& t) { return std::forward<T>(t); }

		template<typename T, typename TT>
		[[nodiscard]] inline static
		auto Pow(T _, TT __) {
			return std::pow(_, __);
		}

		template<typename T0, typename T1, typename... Ts>
		[[nodiscard]] inline static
		typename std::common_type<T0, T1, Ts...>::type Min(T0&& val1, T1&& val2, Ts&&... vs)
		{
			if (val1 < val2)
				return Min(val1, std::forward<Ts>(vs)...);
			else
				return Min(val2, std::forward<Ts>(vs)...);
		}
		
		template<typename T0, typename T1, typename... Ts>
		[[nodiscard]] inline static
		typename std::common_type<T0, T1, Ts...>::type Max(T0&& val1, T1&& val2, Ts&&... vs)
		{
			if (val1 > val2)
				return Max(val1, std::forward<Ts>(vs)...);
			else
				return Max(val2, std::forward<Ts>(vs)...);
		}

		template<typename T>
		[[nodiscard]] inline static
		auto Mean(const T& tmp)
		{
			return std::accumulate(tmp.begin(), tmp.end(), T::value_type(0)) / std::size(tmp);
		}

		template<typename T>
		[[nodiscard]] inline static
		T Clamp(T _, T _Min, T _Max) {
			static_assert(std::is_arithmetic_v<T>);
			return Min(Max(_, _Min), _Max) ;
		}
		template<typename T>
		[[nodiscard]] inline static
		T Abs(T _) {
			static_assert(std::is_arithmetic_v<T>);
			return _ >= T(0) ? _ : -_;
		}
		template<typename T>
		[[nodiscard]] inline static
		auto IsEqual(const T& _1, const T& _2, T _Tolerance) {
			return SMath::Abs(_1 - _2) <= _Tolerance;
		}
		/*
		* @todo Add Curved Interp Support
		*/
		template<typename TLerpingFunction, typename T>
		[[nodiscard]] inline static
		auto InterpF(T _1, T _2, float _Alpha) {
			return TLerpingFunction::template Calculate<T, float>(_1, _2, _Alpha);
		}
		template<typename TLerpingFunction, typename T> 
		[[nodiscard]] inline static
		auto Interp(T _1, T _2, T _Alpha) {
			return TLerpingFunction::template Calculate<T, T>(_1, _2, _Alpha);
		}
		template<typename T>
		[[nodiscard]] inline static
		bool IsInRange(T _, T _Low, T _High) {
			return (_Low <= _ && _ <= _High);
		}
		template<ERotType From, ERotType To> 
		[[nodiscard]] inline static
		float Conv(float _) {
			
			if constexpr (From == ERotType::Degrees && To == ERotType::Radians) { 
				_ = std::fmod(_, 360.0f);
				if (_ < 0)
					_ += 360.0f;
				return _ * PIf / 180.0f;
			}
			if constexpr (From == ERotType::Radians && To == ERotType::Degrees){  
				_ = std::fmod(_, PIf * 2.0f);
				if (_ < 0.0f)
					_ += PIf * 2.0f;
				return _ / (PIf / 180.0f); 
			}
		}
		template<typename T, ETimeType From, ETimeType To>
		[[nodiscard]] inline static
		auto Conv(T _Time) {
			if constexpr (From == To) return _Time;
			if constexpr (To == ETimeType::Day)	return Conv<T, From, ETimeType::Hour>(_Time) / T(24);
			if constexpr (From == ETimeType::Day)	return Conv<T, ETimeType::Hour, To>(_Time * T(24));

			auto FromI = int8_t(From);
			auto ToI = int8_t(To);
			
			if (FromI >= 1) {
				if (ToI >= 10)	return _Time * std::pow(T(10), ToI - FromI);
				else if(ToI <= 0) return Conv<T, From, ETimeType::Seconds>(_Time) / std::pow(T(60), 0 - ToI);
			}

			return Conv<T, ETimeType::Seconds, To>(_Time * std::pow(T(60), 0 - FromI));
		}

		[[nodiscard]] inline static 
		float Radians(float _) { return Conv<ERotType::Degrees, ERotType::Radians>(_); }
		[[nodiscard]] inline static 
		float Degrees(float _) { return Conv<ERotType::Radians, ERotType::Degrees>(_); }
	};
}
