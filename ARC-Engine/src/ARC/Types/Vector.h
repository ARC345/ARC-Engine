#pragma once

#include "TybeBase.h"
#include <type_traits>
#include <initializer_list>
#include <cstring>
#include "spdlog/spdlog.h"

// Enable GLM SIMD intrinsics
#define GLM_FORCE_INTRINSICS
#define GLM_FORCE_SSE2
#define GLM_FORCE_SSE3
#define GLM_FORCE_SSSE3
#define GLM_FORCE_SSE41
#define GLM_FORCE_SSE42
#define GLM_FORCE_AVX
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/component_wise.hpp>

namespace ARC
{
	// ---------------------------------------------------------------
	//  Base: common interface for all TVec types
	// ---------------------------------------------------------------
	namespace Base
	{
		template <class T, size_t N>
		struct TVec_Base : public TTypeBase {
		protected:
			TVec_Base() = default;
		public:
			using Super = TTypeBase;
			using value_type = T;
			using size_type = size_t;
			using type = TVec_Base<value_type, N>;

			TString ToString() const {
				TString x = fmt::format("{}", (*this)[0]);
				for (size_t i = 1; i < N; i++)
					x = fmt::format("{}, {}", x, (*this)[i]);
				return fmt::format("[{}]", x);
			}
		};
	}

	// ---------------------------------------------------------------
	//  Primary template (generic N, T) — for non-standard sizes
	// ---------------------------------------------------------------
	template <size_t N, class T>
	class TVec : public Base::TVec_Base<T, N>
	{
	public:
		using value_type = T;
		using Super = typename Base::TVec_Base<value_type, N>;
		using type = TVec<N, value_type>;
		using type_float = typename std::common_type<value_type, float>::type;
		using size_type = size_t;

		TVec() { std::memset(mData, 0, sizeof(mData)); }
		TVec(const value_type& _) { for (auto& iv : mData) iv = _; }

		value_type& operator[](size_type _) { return mData[_]; }
		const value_type& operator[](size_type _) const { return mData[_]; }

		value_type* Data() { return mData; }
		const value_type* Data() const { return mData; }
		size_type size() const { return N; }
		value_type* begin() { return mData; }
		value_type* end() { return mData + N; }
		const value_type* begin() const { return mData; }
		const value_type* end() const { return mData + N; }

		constexpr bool operator==(const type& _) const {
			for (size_type i = 0; i < N; i++)
				if (mData[i] != _.mData[i]) return false;
			return true;
		}
		constexpr bool operator!=(const type& _) const { return !(*this == _); }

		constexpr type operator+(const type& _) const { type r; for (size_type i = 0; i < N; i++) r.mData[i] = mData[i] + _.mData[i]; return r; }
		constexpr type operator-(const type& _) const { type r; for (size_type i = 0; i < N; i++) r.mData[i] = mData[i] - _.mData[i]; return r; }
		constexpr type operator*(const type& _) const { type r; for (size_type i = 0; i < N; i++) r.mData[i] = mData[i] * _.mData[i]; return r; }
		constexpr type operator/(const type& _) const { type r; for (size_type i = 0; i < N; i++) r.mData[i] = mData[i] / _.mData[i]; return r; }

		constexpr type operator+(const value_type& _) const { return *this + type(_); }
		constexpr type operator-(const value_type& _) const { return *this - type(_); }
		constexpr type operator*(const value_type& _) const { return *this * type(_); }
		constexpr type operator/(const value_type& _) const { return *this / type(_); }

		constexpr void operator+=(const type& _) { for (size_type i = 0; i < N; i++) mData[i] += _.mData[i]; }
		constexpr void operator-=(const type& _) { for (size_type i = 0; i < N; i++) mData[i] -= _.mData[i]; }
		constexpr void operator*=(const type& _) { for (size_type i = 0; i < N; i++) mData[i] *= _.mData[i]; }
		constexpr void operator/=(const type& _) { for (size_type i = 0; i < N; i++) mData[i] /= _.mData[i]; }
		constexpr void operator+=(const value_type& _) { for (size_type i = 0; i < N; i++) mData[i] += _; }
		constexpr void operator-=(const value_type& _) { for (size_type i = 0; i < N; i++) mData[i] -= _; }
		constexpr void operator*=(const value_type& _) { for (size_type i = 0; i < N; i++) mData[i] *= _; }
		constexpr void operator/=(const value_type& _) { for (size_type i = 0; i < N; i++) mData[i] /= _; }

	private:
		value_type mData[N];
	};

	// ---------------------------------------------------------------
	//  TVec<1, T> — Scalar vector
	// ---------------------------------------------------------------
	template <class T>
	class TVec<1, T> : public Base::TVec_Base<T, 1>
	{
	public:
		using Super = Base::TVec_Base<T, 1>;
		using value_type = T;
		using type = TVec<1, value_type>;
		using type_float = typename std::common_type<value_type, float>::type;
		using size_type = size_t;

		union { value_type x, r; };

		TVec() : x(0) {}
		TVec(const value_type& _x) : x(_x) {}

		value_type* Data() { return &x; }
		const value_type* Data() const { return &x; }
		size_type size() const { return 1; }
		value_type* begin() { return &x; }
		value_type* end() { return &x + 1; }
		const value_type* begin() const { return &x; }
		const value_type* end() const { return &x + 1; }

		void DrawGuiControl(const char* ID, float pColumnWidth);

		constexpr bool operator==(const type& _) const { return x == _.x; }
		constexpr bool operator!=(const type& _) const { return x != _.x; }

		constexpr type operator+(const type& _) const { return type(x + _.x); }
		constexpr type operator-(const type& _) const { return type(x - _.x); }
		constexpr type operator*(const type& _) const { return type(x * _.x); }
		constexpr type operator/(const type& _) const { return type(x / _.x); }
		constexpr type operator+(const value_type _) const { return type(x + _); }
		constexpr type operator-(const value_type _) const { return type(x - _); }
		constexpr type operator*(const value_type _) const { return type(x * _); }
		constexpr type operator/(const value_type _) const { return type(x / _); }

		constexpr void operator+=(const type& _) { x += _.x; }
		constexpr void operator-=(const type& _) { x -= _.x; }
		constexpr void operator*=(const type& _) { x *= _.x; }
		constexpr void operator/=(const type& _) { x /= _.x; }
		constexpr void operator+=(const value_type _) { x += _; }
		constexpr void operator-=(const value_type _) { x -= _; }
		constexpr void operator*=(const value_type _) { x *= _; }
		constexpr void operator/=(const value_type _) { x /= _; }

		type_float Dist(const type& _) const { return type_float(x > _.x ? x - _.x : _.x - x); }
		type_float Length() const { return x; }
		type Normalize() const { return type(x / Length()); }
		bool AlmostEqual(const type& _, float _Tolerance) const { return (x > _.x ? x - _.x : _.x - x) <= _Tolerance; }

		static const type& ZeroVector() { static type rval(0); return rval; }
		static const type& OneVector() { static type rval(1); return rval; }
	};

	// ---------------------------------------------------------------
	//  TVec<2, T> — Non-float specializations (int, uint, etc.)
	//  Float specialization follows separately with GLM backing.
	// ---------------------------------------------------------------
	template <class T>
	class TVec<2, T> : public Base::TVec_Base<T, 2>
	{
	public:
		using Super = Base::TVec_Base<T, 2>;
		using value_type = T;
		using type = TVec<2, value_type>;
		using type_float = typename std::common_type<value_type, float>::type;
		using size_type = size_t;

		union { value_type x, r; };
		union { value_type y, g; };

		TVec() : x(0), y(0) {}
		TVec(const value_type& _x, const value_type& _y) : x(_x), y(_y) {}
		TVec(const value_type& _) : x(_), y(_) {}

		value_type* Data() { return &x; }
		const value_type* Data() const { return &x; }
		size_type size() const { return 2; }
		value_type* begin() { return &x; }
		value_type* end() { return &y + 1; }
		const value_type* begin() const { return &x; }
		const value_type* end() const { return &y + 1; }

		constexpr bool operator==(const type& _) const { return x == _.x && y == _.y; }
		constexpr bool operator!=(const type& _) const { return !(*this == _); }

		constexpr type operator+(const type& _) const { return { x + _.x, y + _.y }; }
		constexpr type operator-(const type& _) const { return { x - _.x, y - _.y }; }
		constexpr type operator*(const type& _) const { return { x * _.x, y * _.y }; }
		constexpr type operator/(const type& _) const { return { x / _.x, y / _.y }; }
		constexpr type operator+(const value_type _) const { return { x + _, y + _ }; }
		constexpr type operator-(const value_type _) const { return { x - _, y - _ }; }
		constexpr type operator*(const value_type _) const { return { x * _, y * _ }; }
		constexpr type operator/(const value_type _) const { return { x / _, y / _ }; }

		constexpr void operator+=(const type& _) { x += _.x; y += _.y; }
		constexpr void operator-=(const type& _) { x -= _.x; y -= _.y; }
		constexpr void operator*=(const type& _) { x *= _.x; y *= _.y; }
		constexpr void operator/=(const type& _) { x /= _.x; y /= _.y; }
		constexpr void operator+=(const value_type _) { x += _; y += _; }
		constexpr void operator-=(const value_type _) { x -= _; y -= _; }
		constexpr void operator*=(const value_type _) { x *= _; y *= _; }
		constexpr void operator/=(const value_type _) { x /= _; y /= _; }

		void DrawGuiControl(const char* pID, float pColumnWidth, type pDefaults);

		value_type MinComponent() const { return (x < y) ? x : y; }
		value_type MaxComponent() const { return (x > y) ? x : y; }

		bool IsWithinBounds(const type& pMin, const type& pMax) const {
			return (x >= pMin.x && x <= pMax.x) && (y >= pMin.y && y <= pMax.y);
		}

		type_float Dist(const type& _) const {
			type_float dx = type_float(x - _.x);
			type_float dy = type_float(y - _.y);
			return type_float(std::sqrt(dx * dx + dy * dy));
		}
		type_float DistSqr(const type& _) const {
			type_float dx = type_float(x - _.x);
			type_float dy = type_float(y - _.y);
			return dx * dx + dy * dy;
		}
		type_float Length() const { return Dist(ZeroVector()); }
		type Normalize() const { type_float len = Length(); return (len > 0) ? type(T(x / len), T(y / len)) : *this; }
		bool AlmostEqual(const type& _, float _Tolerance) const {
			return (x > _.x ? (x - _.x) : (_.x - x)) <= _Tolerance &&
			       (y > _.y ? (y - _.y) : (_.y - y)) <= _Tolerance;
		}

		static const type& ZeroVector() { static type rval(0); return rval; }
		static const type& OneVector() { static type rval(1); return rval; }
	};

	// ---------------------------------------------------------------
	//  TVec<3, T> — Non-float specializations
	// ---------------------------------------------------------------
	template <class T>
	class TVec<3, T> : public Base::TVec_Base<T, 3>
	{
	public:
		using Super = Base::TVec_Base<T, 3>;
		using value_type = T;
		using type = TVec<3, T>;
		using type_float = typename std::common_type<value_type, float>::type;
		using size_type = size_t;

		union { value_type x, r; };
		union { value_type y, g; };
		union { value_type z, b; };

		TVec() : x(0), y(0), z(0) {}
		TVec(const value_type& _x, const value_type& _y, const value_type& _z) : x(_x), y(_y), z(_z) {}
		TVec(const value_type _) : x(_), y(_), z(_) {}

		value_type* Data() { return &x; }
		const value_type* Data() const { return &x; }
		size_type size() const { return 3; }
		value_type* begin() { return &x; }
		value_type* end() { return &z + 1; }
		const value_type* begin() const { return &x; }
		const value_type* end() const { return &z + 1; }

		constexpr bool operator==(const type& _) const { return x == _.x && y == _.y && z == _.z; }
		constexpr bool operator!=(const type& _) const { return !(*this == _); }

		constexpr type operator+(const type& _) const { return { x + _.x, y + _.y, z + _.z }; }
		constexpr type operator-(const type& _) const { return { x - _.x, y - _.y, z - _.z }; }
		constexpr type operator*(const type& _) const { return { x * _.x, y * _.y, z * _.z }; }
		constexpr type operator/(const type& _) const { return { x / _.x, y / _.y, z / _.z }; }
		constexpr type operator+(const value_type _) const { return { x + _, y + _, z + _ }; }
		constexpr type operator-(const value_type _) const { return { x - _, y - _, z - _ }; }
		constexpr type operator*(const value_type _) const { return { x * _, y * _, z * _ }; }
		constexpr type operator/(const value_type _) const { return { x / _, y / _, z / _ }; }

		constexpr void operator+=(const type& _) { x += _.x; y += _.y; z += _.z; }
		constexpr void operator-=(const type& _) { x -= _.x; y -= _.y; z -= _.z; }
		constexpr void operator*=(const type& _) { x *= _.x; y *= _.y; z *= _.z; }
		constexpr void operator/=(const type& _) { x /= _.x; y /= _.y; z /= _.z; }
		constexpr void operator+=(const value_type _) { x += _; y += _; z += _; }
		constexpr void operator-=(const value_type _) { x -= _; y -= _; z -= _; }
		constexpr void operator*=(const value_type _) { x *= _; y *= _; z *= _; }
		constexpr void operator/=(const value_type _) { x /= _; y /= _; z /= _; }

		void DrawGuiControl(const char* pID, float pColumnWidth, type pDefaults);

		value_type MinComponent() const { return (x < y) ? (x < z ? x : z) : (y < z ? y : z); }
		value_type MaxComponent() const { return (x > y) ? (x > z ? x : z) : (y > z ? y : z); }

		type_float Dist(const type& _) const {
			type_float dx = type_float(x - _.x);
			type_float dy = type_float(y - _.y);
			type_float dz = type_float(z - _.z);
			return type_float(std::sqrt(dx * dx + dy * dy + dz * dz));
		}
		type_float DistSqr(const type& _) const {
			type_float dx = type_float(x - _.x);
			type_float dy = type_float(y - _.y);
			type_float dz = type_float(z - _.z);
			return dx * dx + dy * dy + dz * dz;
		}
		type_float Length() const { return Dist(ZeroVector()); }
		type Normalize() const { type_float len = Length(); return (len > 0) ? type(T(x / len), T(y / len), T(z / len)) : *this; }
		type Cross(const type& _1, const type& _2) {
			return type(
				_1.y * _2.z - _2.y * _1.z,
				_1.z * _2.x - _2.z * _1.x,
				_1.x * _2.y - _2.x * _1.y
			);
		}
		bool AlmostEqual(const type& _, float _Tolerance) const {
			return (x > _.x ? (x - _.x) : (_.x - x)) <= _Tolerance &&
			       (y > _.y ? (y - _.y) : (_.y - y)) <= _Tolerance &&
			       (z > _.z ? (z - _.z) : (_.z - z)) <= _Tolerance;
		}

		type Mask(type _) { return *this * _; }

		static const type& ZeroVector() { static type rval(0); return rval; }
		static const type& OneVector() { static type rval(1); return rval; }
	};

	// ---------------------------------------------------------------
	//  TVec<4, T> — Non-float specializations
	// ---------------------------------------------------------------
	template <class T>
	class TVec<4, T> : public Base::TVec_Base<T, 4>
	{
	public:
		using Super = Base::TVec_Base<T, 4>;
		using value_type = T;
		using type = TVec<4, T>;
		using type_float = typename std::common_type<value_type, float>::type;
		using size_type = size_t;

		union { value_type x, r; };
		union { value_type y, g; };
		union { value_type z, b; };
		union { value_type w, a; };

		TVec() : x(0), y(0), z(0), w(0) {}
		TVec(const value_type& _x, const value_type& _y, const value_type& _z, const value_type& _w) : x(_x), y(_y), z(_z), w(_w) {}
		TVec(const value_type _) : x(_), y(_), z(_), w(_) {}

		value_type* Data() { return &x; }
		const value_type* Data() const { return &x; }
		size_type size() const { return 4; }
		value_type* begin() { return &x; }
		value_type* end() { return &w + 1; }
		const value_type* begin() const { return &x; }
		const value_type* end() const { return &w + 1; }

		constexpr bool operator==(const type& _) const { return x == _.x && y == _.y && z == _.z && w == _.w; }
		constexpr bool operator!=(const type& _) const { return !(*this == _); }

		constexpr type operator+(const type& _) const { return { x + _.x, y + _.y, z + _.z, w + _.w }; }
		constexpr type operator-(const type& _) const { return { x - _.x, y - _.y, z - _.z, w - _.w }; }
		constexpr type operator*(const type& _) const { return { x * _.x, y * _.y, z * _.z, w * _.w }; }
		constexpr type operator/(const type& _) const { return { x / _.x, y / _.y, z / _.z, w / _.w }; }
		constexpr type operator+(const value_type _) const { return { x + _, y + _, z + _, w + _ }; }
		constexpr type operator-(const value_type _) const { return { x - _, y - _, z - _, w - _ }; }
		constexpr type operator*(const value_type _) const { return { x * _, y * _, z * _, w * _ }; }
		constexpr type operator/(const value_type _) const { return { x / _, y / _, z / _, w / _ }; }

		constexpr void operator+=(const type& _) { x += _.x; y += _.y; z += _.z; w += _.w; }
		constexpr void operator-=(const type& _) { x -= _.x; y -= _.y; z -= _.z; w -= _.w; }
		constexpr void operator*=(const type& _) { x *= _.x; y *= _.y; z *= _.z; w *= _.w; }
		constexpr void operator/=(const type& _) { x /= _.x; y /= _.y; z /= _.z; w /= _.w; }
		constexpr void operator+=(const value_type _) { x += _; y += _; z += _; w += _; }
		constexpr void operator-=(const value_type _) { x -= _; y -= _; z -= _; w -= _; }
		constexpr void operator*=(const value_type _) { x *= _; y *= _; z *= _; w *= _; }
		constexpr void operator/=(const value_type _) { x /= _; y /= _; z /= _; w /= _; }

		void DrawGuiControl(const char* pID, float pColumnWidth, type pDefaults);

		value_type MinComponent() const {
			value_type m = (x < y) ? x : y;
			m = (z < m) ? z : m;
			m = (w < m) ? w : m;
			return m;
		}
		value_type MaxComponent() const {
			value_type m = (x > y) ? x : y;
			m = (z > m) ? z : m;
			m = (w > m) ? w : m;
			return m;
		}

		type_float Dist(const type& _) const {
			type_float dx = type_float(x - _.x);
			type_float dy = type_float(y - _.y);
			type_float dz = type_float(z - _.z);
			type_float dw = type_float(w - _.w);
			return type_float(std::sqrt(dx * dx + dy * dy + dz * dz + dw * dw));
		}
		type_float DistSqr(const type& _) const {
			type_float dx = type_float(x - _.x);
			type_float dy = type_float(y - _.y);
			type_float dz = type_float(z - _.z);
			type_float dw = type_float(w - _.w);
			return dx * dx + dy * dy + dz * dz + dw * dw;
		}
		type_float Length() const { return Dist(ZeroVector()); }
		type Normalize() const { type_float len = Length(); return (len > 0) ? type(T(x / len), T(y / len), T(z / len), T(w / len)) : *this; }
		bool AlmostEqual(const type& _, float _Tolerance) const {
			return (x > _.x ? (x - _.x) : (_.x - x)) <= _Tolerance &&
			       (y > _.y ? (y - _.y) : (_.y - y)) <= _Tolerance &&
			       (z > _.z ? (z - _.z) : (_.z - z)) <= _Tolerance &&
			       (w > _.w ? (w - _.w) : (_.w - w)) <= _Tolerance;
		}

		static const type& ZeroVector() { static type rval(0); return rval; }
		static const type& OneVector() { static type rval(1); return rval; }
	};

	// ===============================================================
	//  GLM-BACKED FLOAT SPECIALIZATIONS
	//  All operations delegate to GLM for SIMD-optimized math.
	//  Same API surface as non-float variants.
	// ===============================================================

	// ---------------------------------------------------------------
	//  TVec<2, float> — glm::vec2 backed
	// ---------------------------------------------------------------
	template <>
	class TVec<2, float> : public Base::TVec_Base<float, 2>
	{
	public:
		using Super = Base::TVec_Base<float, 2>;
		using value_type = float;
		using type = TVec<2, value_type>;
		using type_float = float;
		using size_type = size_t;

		// Data layout matches glm::vec2 exactly: two floats
		union {
			struct { float x, y; };
			struct { float r, g; };
			glm::vec2 data;
		};

		TVec() : data(0.0f) {}
		TVec(float _x, float _y) : data(_x, _y) {}
		TVec(float _) : data(_) {}
		TVec(const glm::vec2& _) : data(_) {}

		float* Data() { return &x; }
		const float* Data() const { return &x; }
		size_type size() const { return 2; }
		float* begin() { return &x; }
		float* end() { return &y + 1; }
		const float* begin() const { return &x; }
		const float* end() const { return &y + 1; }

		// Implicit conversion to/from glm (internal use only)
		operator       glm::vec2& ()       { return data; }
		operator const glm::vec2& () const { return data; }

		// --- Operators ---
		bool operator==(const type& _) const { return data == _.data; }
		bool operator!=(const type& _) const { return data != _.data; }

		type operator+(const type& _) const { return type(data + _.data); }
		type operator-(const type& _) const { return type(data - _.data); }
		type operator*(const type& _) const { return type(data * _.data); }
		type operator/(const type& _) const { return type(data / _.data); }
		type operator+(float _) const { return type(data + _); }
		type operator-(float _) const { return type(data - _); }
		type operator*(float _) const { return type(data * _); }
		type operator/(float _) const { return type(data / _); }

		type& operator+=(const type& _) { data += _.data; return *this; }
		type& operator-=(const type& _) { data -= _.data; return *this; }
		type& operator*=(const type& _) { data *= _.data; return *this; }
		type& operator/=(const type& _) { data /= _.data; return *this; }
		type& operator+=(float _) { data += _; return *this; }
		type& operator-=(float _) { data -= _; return *this; }
		type& operator*=(float _) { data *= _; return *this; }
		type& operator/=(float _) { data /= _; return *this; }

		// --- Math ---
		void DrawGuiControl(const char* pID, float pColumnWidth, type pDefaults);

		float MinComponent() const { return glm::compMin(data); }
		float MaxComponent() const { return glm::compMax(data); }

		bool IsWithinBounds(const type& pMin, const type& pMax) const {
			return glm::all(glm::greaterThanEqual(data, pMin.data)) &&
			       glm::all(glm::lessThanEqual(data, pMax.data));
		}

		float Dist(const type& _) const { return glm::distance(data, _.data); }
		float DistSqr(const type& _) const { return glm::distance2(data, _.data); }
		float Length() const { return glm::length(data); }
		type Normalize() const { return type(glm::normalize(data)); }
		bool AlmostEqual(const type& _, float _Tolerance) const {
			return glm::all(glm::epsilonEqual(data, _.data, _Tolerance));
		}

		static const type& ZeroVector() { static type rval(0.0f); return rval; }
		static const type& OneVector() { static type rval(1.0f); return rval; }
	};

	// ---------------------------------------------------------------
	//  TVec<3, float> — glm::vec3 backed
	// ---------------------------------------------------------------
	template <>
	class TVec<3, float> : public Base::TVec_Base<float, 3>
	{
	public:
		using Super = Base::TVec_Base<float, 3>;
		using value_type = float;
		using type = TVec<3, value_type>;
		using type_float = float;
		using size_type = size_t;

		union {
			struct { float x, y, z; };
			struct { float r, g, b; };
			glm::vec3 data;
		};

		TVec() : data(0.0f) {}
		TVec(float _x, float _y, float _z) : data(_x, _y, _z) {}
		TVec(float _) : data(_) {}
		TVec(const glm::vec3& _) : data(_) {}

		float* Data() { return &x; }
		const float* Data() const { return &x; }
		size_type size() const { return 3; }
		float* begin() { return &x; }
		float* end() { return &z + 1; }
		const float* begin() const { return &x; }
		const float* end() const { return &z + 1; }

		operator       glm::vec3& ()       { return data; }
		operator const glm::vec3& () const { return data; }

		bool operator==(const type& _) const { return data == _.data; }
		bool operator!=(const type& _) const { return data != _.data; }

		type operator+(const type& _) const { return type(data + _.data); }
		type operator-(const type& _) const { return type(data - _.data); }
		type operator*(const type& _) const { return type(data * _.data); }
		type operator/(const type& _) const { return type(data / _.data); }
		type operator+(float _) const { return type(data + _); }
		type operator-(float _) const { return type(data - _); }
		type operator*(float _) const { return type(data * _); }
		type operator/(float _) const { return type(data / _); }

		type& operator+=(const type& _) { data += _.data; return *this; }
		type& operator-=(const type& _) { data -= _.data; return *this; }
		type& operator*=(const type& _) { data *= _.data; return *this; }
		type& operator/=(const type& _) { data /= _.data; return *this; }
		type& operator+=(float _) { data += _; return *this; }
		type& operator-=(float _) { data -= _; return *this; }
		type& operator*=(float _) { data *= _; return *this; }
		type& operator/=(float _) { data /= _; return *this; }

		void DrawGuiControl(const char* pID, float pColumnWidth, type pDefaults);

		float MinComponent() const { return glm::compMin(data); }
		float MaxComponent() const { return glm::compMax(data); }

		float Dist(const type& _) const { return glm::distance(data, _.data); }
		float DistSqr(const type& _) const { return glm::distance2(data, _.data); }
		float Length() const { return glm::length(data); }
		type Normalize() const { return type(glm::normalize(data)); }
		type Cross(const type& _1, const type& _2) { return type(glm::cross(_1.data, _2.data)); }
		bool AlmostEqual(const type& _, float _Tolerance) const {
			return glm::all(glm::epsilonEqual(data, _.data, _Tolerance));
		}

		type Mask(type _) { return type(data * _.data); }

		static const type& ZeroVector() { static type rval(0.0f); return rval; }
		static const type& OneVector() { static type rval(1.0f); return rval; }
	};

	// ---------------------------------------------------------------
	//  TVec<4, float> — glm::vec4 backed
	// ---------------------------------------------------------------
	template <>
	class TVec<4, float> : public Base::TVec_Base<float, 4>
	{
	public:
		using Super = Base::TVec_Base<float, 4>;
		using value_type = float;
		using type = TVec<4, value_type>;
		using type_float = float;
		using size_type = size_t;

		union {
			struct { float x, y, z, w; };
			struct { float r, g, b, a; };
			glm::vec4 data;
		};

		TVec() : data(0.0f) {}
		TVec(float _x, float _y, float _z, float _w) : data(_x, _y, _z, _w) {}
		TVec(float _) : data(_) {}
		TVec(const glm::vec4& _) : data(_) {}

		float* Data() { return &x; }
		const float* Data() const { return &x; }
		size_type size() const { return 4; }
		float* begin() { return &x; }
		float* end() { return &w + 1; }
		const float* begin() const { return &x; }
		const float* end() const { return &w + 1; }

		operator       glm::vec4& ()       { return data; }
		operator const glm::vec4& () const { return data; }

		bool operator==(const type& _) const { return data == _.data; }
		bool operator!=(const type& _) const { return data != _.data; }

		type operator+(const type& _) const { return type(data + _.data); }
		type operator-(const type& _) const { return type(data - _.data); }
		type operator*(const type& _) const { return type(data * _.data); }
		type operator/(const type& _) const { return type(data / _.data); }
		type operator+(float _) const { return type(data + _); }
		type operator-(float _) const { return type(data - _); }
		type operator*(float _) const { return type(data * _); }
		type operator/(float _) const { return type(data / _); }

		type& operator+=(const type& _) { data += _.data; return *this; }
		type& operator-=(const type& _) { data -= _.data; return *this; }
		type& operator*=(const type& _) { data *= _.data; return *this; }
		type& operator/=(const type& _) { data /= _.data; return *this; }
		type& operator+=(float _) { data += _; return *this; }
		type& operator-=(float _) { data -= _; return *this; }
		type& operator*=(float _) { data *= _; return *this; }
		type& operator/=(float _) { data /= _; return *this; }

		void DrawGuiControl(const char* pID, float pColumnWidth, type pDefaults);

		float MinComponent() const { return glm::compMin(data); }
		float MaxComponent() const { return glm::compMax(data); }

		float Dist(const type& _) const { return glm::distance(data, _.data); }
		float DistSqr(const type& _) const { return glm::distance2(data, _.data); }
		float Length() const { return glm::length(data); }
		type Normalize() const { return type(glm::normalize(data)); }
		bool AlmostEqual(const type& _, float _Tolerance) const {
			return glm::all(glm::epsilonEqual(data, _.data, _Tolerance));
		}

		static const type& ZeroVector() { static type rval(0.0f); return rval; }
		static const type& OneVector() { static type rval(1.0f); return rval; }
	};

	// ---------------------------------------------------------------
	//  Type aliases
	// ---------------------------------------------------------------
	template<typename T> using TVec1 = TVec<1, T>;
	template<typename T> using TVec2 = TVec<2, T>;
	template<typename T> using TVec3 = TVec<3, T>;
	template<typename T> using TVec4 = TVec<4, T>;

	using FVec1 = TVec1<float>;
	using FVec2 = TVec2<float>;
	using FVec3 = TVec3<float>;
	using FVec4 = TVec4<float>;

	using IVec1 = TVec1<int>;
	using IVec2 = TVec2<int>;
	using IVec3 = TVec3<int>;
	using IVec4 = TVec4<int>;

	using UIVec1 = TVec1<unsigned int>;
	using UIVec2 = TVec2<unsigned int>;
	using UIVec3 = TVec3<unsigned int>;
	using UIVec4 = TVec4<unsigned int>;
};
