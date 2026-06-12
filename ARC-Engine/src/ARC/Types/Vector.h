#pragma once

#include "TybeBase.h"
#include <type_traits>
#include <initializer_list>
#include "ARC/Helpers/Math.h"
#include "spdlog/spdlog.h"

namespace arc
{
	#define VM_FUNC [[nodiscard]] inline constexpr

	namespace Base
	{
		template <class T, size_t N>
		struct ARC_API TVec_Base : public type_base {
		protected:
			TVec_Base() = default;
		public:
			using Super = type_base;
			using value_type = T;
			using size_type = size_t;
			using type = TVec_Base<value_type, N>;

			value_type& operator[](size_type _)	{ return begin()[_]; };
			const value_type& operator[](size_type _) const	{ return begin()[_]; };

			inline value_type* Data() { return (value_type*)this; }
			inline const value_type* Data() const { return (value_type*)this; }

			VM_FUNC size_type size() { return N; }
			value_type* begin() { return (value_type*)this; }
			value_type* end() {	return begin() + N;	}

			const value_type* begin() const { return (value_type*)this; }
			const value_type* end() const { return begin() + N; }
			string to_string() {
				string x = fmt::format("{}", *begin()) ;
				for (size_t i = 1; i < N; i++)
				{
					x = fmt::format("{}, {}", x, (*this)[i]);
				}
				return fmt::format("[{}]", x);
			}
		};
	}
	template <size_t N, class T>
	class ARC_API vec : public Base::TVec_Base<T, N>
	{
	public:
		using value_type = T;
		using Super = typename Base::TVec_Base<value_type, N>;
		using type = vec<N, value_type>;
		using type_float = typename std::common_type<value_type, float>::type;
		using size_type = size_t;

		vec() : data_() {};
		vec(const value_type* _) : data_(_) {};
		vec(const value_type& _) {	for (auto& iv : *this)	iv=_; };

		vec(value_type pData[N]) : data_(pData) {}

		value_type& operator[](size_type _)	{ return data_[_]; };
		const value_type& operator[](size_type _) const { return data_[_]; };

		constexpr bool operator==(const type& _)
		{
			for (size_type i = 0; i < N; i++)
				if (this->Data()[i] != _.Data()[i])
					return false;
			return true;
		}
		constexpr void operator+=(const type& _) { this = *this + _; }
		constexpr void operator-=(const type& _) { this = *this - _; }
		constexpr void operator*=(const type& _) { this = *this * _; }
		constexpr void operator/=(const type& _) { this = *this / _; }
		
		constexpr type operator+(const type& _) const
		{
			type rval;
			for (size_type i = 0; i < N; i++)
				rval[i] = Data()[i] + _.Data()[i];
			return rval;
		}
		constexpr type operator-(const type& _) const
		{
			type rval;
			for (size_type i = 0; i < N; i++)
				rval[i] = Data()[i] - _.Data()[i];
			return rval;
		}
		constexpr type operator*(const type& _) const
		{
			type rval;
			for (size_type i = 0; i < N; i++)
				rval[i] = this->Data()[i] - _.Data()[i];
			return rval;
		}
		constexpr type operator/(const type& _) const
		{
			type rval;
			for (size_type i = 0; i < N; i++)
				rval[i] = this->Data()[i] / _.Data()[i];
			return rval;
		}
		constexpr void operator+=(const value_type& _) { *this += type(_); }
		constexpr void operator-=(const value_type& _) { *this -= type(_); }
		constexpr void operator*=(const value_type& _) { *this *= type(_); }
		constexpr void operator/=(const value_type& _) { *this /= type(_); }

		constexpr type operator+(const value_type& _) const { return *this+type(_); }
		constexpr type operator-(const value_type& _) const { return *this-type(_); }
		constexpr type operator*(const value_type& _) const { return *this*type(_); }
		constexpr type operator/(const value_type& _) const { return *this/type(_); }

		inline value_type* Data() { return data_; }
		inline const value_type* Data() const { return data_; }

	private:
		value_type data_[N];
	};

	template <class T>
	class ARC_API vec<1, T> : public Base::TVec_Base<T, 1>
	{
	public:
		using Super = Base::TVec_Base<T, 1>;

		using value_type = T;
		using type = vec<1, value_type>;
		using type_float = typename std::common_type<value_type, float>::type;
		using size_type = size_t;

		union { value_type x, r; };

		vec() {}
		vec(const value_type& _x) : x(_x){}

		void draw_gui_control(const char* ID, float pColumnWidth);

		VM_FUNC type_float Dist(const type& _) const { return math::Abs(_-*this); }
		VM_FUNC type_float Length() const { return *this; }
		VM_FUNC type Normalize() const { return this / Length(); }
		VM_FUNC bool AlmostEqual(const type& _, float tollerance) const { return math::is_equal(this, _, tollerance); }

		[[nodiscard]] static
		const type& ZeroVector() { static type rval = type(0); return rval; }
		[[nodiscard]] static
		const type& OneVector() { static type rval = type(1); return rval; }
	};

	template <class T>
	class ARC_API vec<2, T> : public Base::TVec_Base<T, 2>
	{
		ARC_TYPE();
		
	public:
		
		using Super = Base::TVec_Base<T, 2>;

		using value_type = T;
		using type = vec<2, value_type>;
		using type_float = typename std::common_type<value_type, float>::type;
		using size_type = size_t;

		union { value_type x, r; };
		union { value_type y, g; };
		
		vec() {}
		vec(const value_type& _x, const value_type& _y) : x(_x), y(_y) {}
		vec(const value_type& _) : x(_), y(_) {}

		constexpr bool operator==(const type& _) const { return x == _.x && y == _.y; }
		constexpr bool operator!=(const type& _) const { return !(*this == _); }

		constexpr type operator+(const type& _) const { return { x + _.x, y + _.y }; }
		constexpr type operator-(const type& _) const { return { x - _.x, y - _.y }; }
		constexpr type operator/(const type& _) const { return { x / _.x, y / _.y }; }
		constexpr type operator*(const type& _) const { return { x * _.x, y * _.y }; }
		constexpr type operator+(const value_type _) const { return { x + _, y + _ }; }
		constexpr type operator-(const value_type _) const { return { x - _, y - _ }; }
		constexpr type operator/(const value_type _) const { return { x / _, y / _ }; }
		constexpr type operator*(const value_type _) const { return { x * _, y * _ }; }

		constexpr void operator+=(const type& _) { x += _.x; y += _.y; }
		constexpr void operator-=(const type& _) { x -= _.x; y -= _.y; }
		constexpr void operator/=(const type& _) { x /= _.x; y /= _.y; }
		constexpr void operator*=(const type& _) { x *= _.x; y *= _.y; }
		constexpr void operator+=(const value_type _) { x += _; y += _; }
		constexpr void operator-=(const value_type _) { x -= _; y -= _; }
		constexpr void operator/=(const value_type _) { x /= _; y /= _; }
		constexpr void operator*=(const value_type _) { x *= _; y *= _; }

		void draw_gui_control(const char* pID, float pColumnWidth, type pDefaults);
		
		VM_FUNC value_type MinComponent() const { return math::Min(x, y); }
		VM_FUNC value_type MaxComponent() const { return math::Max(x, y); }

		VM_FUNC bool is_within_bounds(const type& pMin, const type& pMax)
		{
			return math::is_in_range(x, pMin.x, pMax.x) && math::is_in_range(y, pMin.y, pMax.y);
		}

		VM_FUNC type_float Dist(const type& _) const { return math::Dist(*this, _); }
		VM_FUNC type_float DistSqr(const type& _) const { return math::DistSqr(*this, _); }
		VM_FUNC type_float Length() const { return Dist(ZeroVector()); }
		VM_FUNC type Normalize() const { return this / Length(); }
		VM_FUNC bool AlmostEqual(const type& _, float tollerance) const { return math::is_equal(*this, _, tollerance); }
		[[nodiscard]] static
		const type& ZeroVector() { static type rval = type(0); return rval; }
		[[nodiscard]] static
		const type& OneVector() { static type rval = type(1); return rval; }
	private:
	};
	
	template <class T>
	class ARC_API vec<3, T> : public Base::TVec_Base<T, 3>
	{
	public:
		using Super = Base::TVec_Base<T, 3>;

		using value_type = T;
		using type = vec<3, T>;
		using type_float = typename std::common_type<value_type, float>::type;
		using size_type = size_t;

		union { value_type x, r; };
		union { value_type y, g; };
		union { value_type z, b; };

		vec() {}
		vec(const value_type& _x, const value_type& _y, const value_type& _z) : x(_x), y(_y), z(_z) {}
		vec(const value_type _) : x(_), y(_), z(_) {}

		constexpr bool operator==(const type& _) const { return x == _.x && y == _.y && z == _.z; }
		constexpr bool operator!=(const type& _) const { return !(*this == _); }

		constexpr type operator+(const type& _) const { return { x + _.x, y + _.y, z + _.z }; }
		constexpr type operator-(const type& _) const { return { x - _.x, y - _.y, z - _.z }; }
		constexpr type operator/(const type& _) const { return { x / _.x, y / _.y, z / _.z }; }
		constexpr type operator*(const type& _) const { return { x * _.x, y * _.y, z * _.z }; }
		constexpr type operator+(const value_type _) const { return { x + _, y + _, z + _ }; }
		constexpr type operator-(const value_type _) const { return { x - _, y - _, z - _ }; }
		constexpr type operator/(const value_type _) const { return { x / _, y / _, z / _ }; }
		constexpr type operator*(const value_type _) const { return { x * _, y * _, z * _ }; }

		constexpr void operator+=(const type& _) { x += _.x; y += _.y; z += _.z; }
		constexpr void operator-=(const type& _) { x -= _.x; y -= _.y; z -= _.z; }
		constexpr void operator/=(const type& _) { x /= _.x; y /= _.y; z /= _.z; }
		constexpr void operator*=(const type& _) { x *= _.x; y *= _.y; z *= _.z; }
		constexpr void operator+=(const value_type _) { x += _; y += _; z += _; }
		constexpr void operator-=(const value_type _) { x -= _; y -= _; z -= _; }
		constexpr void operator/=(const value_type _) { x /= _; y /= _; z /= _; }
		constexpr void operator*=(const value_type _) { x *= _; y *= _; z *= _; }

		inline value_type* Data() { return &x; }
		inline const value_type* Data() const { return &x; }

		void draw_gui_control(const char* pID, float pColumnWidth, type pDefaults);

		VM_FUNC value_type MinComponent() const { return math::Min(x, y, z); }
		VM_FUNC value_type MaxComponent() const { return math::Max(x, y, z); }

		VM_FUNC type_float Dist(const type& _) const { return math::Dist(*this, _); }
		VM_FUNC type_float DistSqr(const type& _) const { return math::DistSqr(*this, _); }
		VM_FUNC type_float Length() const { return Dist(ZeroVector()); }

		VM_FUNC type Cross(const type& _1, const type& _2) {
			return (_1.y * _2.z - _2.y * _1.z,
				_1.z * _2.x - _2.z * _1.x,
				_1.x * _2.y - _2.x * _1.y);
		}

		VM_FUNC type Normalize() const { return *this / Length(); }
		VM_FUNC bool AlmostEqual(const type& _, float tollerance) const { return math::is_equal(*this, _, tollerance); }

		VM_FUNC type Mask(type _) { return *this * _; }
		[[nodiscard]] static
		const type& ZeroVector() { static type rval = type(0); return rval; }
		[[nodiscard]] static
		const type& OneVector() { static type rval = type(1); return rval; }
	private:
	};

	template <class T>
	class ARC_API vec<4, T> : public Base::TVec_Base<T, 4>
	{
	public:
		using Super = Base::TVec_Base<T, 4>;

		using value_type = T;
		using type = vec<4, T>;
		using type_float = typename std::common_type<value_type, float>::type;
		using size_type = size_t;

		union { value_type x, r; };
		union { value_type y, g; };
		union { value_type z, b; };
		union { value_type w, a; };

		vec() {}
		vec(const value_type& _x, const value_type& _y, const value_type& _z, const value_type& _w) : x(_x), y(_y), z(_z), w(_w) {}
		vec(const value_type _) : x(_), y(_), z(_), w(_) {}

		constexpr bool operator==(const type& _) const { return x == _.x && y == _.y && z == _.z, w == _.w; }
		constexpr bool operator!=(const type& _) const { return !(*this == _); }

		constexpr type operator+(const type& _) const { return { x + _.x, y + _.y, z + _.z, w + _.w }; }
		constexpr type operator-(const type& _) const { return { x - _.x, y - _.y, z - _.z, w - _.w }; }
		constexpr type operator/(const type& _) const { return { x / _.x, y / _.y, z / _.z, w / _.w }; }
		constexpr type operator*(const type& _) const { return { x * _.x, y * _.y, z * _.z, w * _.w }; }
		constexpr type operator+(const value_type _) const { return { x + _, y + _, z + _, w + _ }; }
		constexpr type operator-(const value_type _) const { return { x - _, y - _, z - _, w - _ }; }
		constexpr type operator/(const value_type _) const { return { x / _, y / _, z / _, w / _ }; }
		constexpr type operator*(const value_type _) const { return { x * _, y * _, z * _, w * _ }; }

		constexpr void operator-=(const type& _) { x -= _.x; y -= _.y; z -= _.z; w -= _.w; }
		constexpr void operator/=(const type& _) { x /= _.x; y /= _.y; z /= _.z; w /= _.w; }
		constexpr void operator*=(const type& _) { x *= _.x; y *= _.y; z *= _.z; w *= _.w; }
		constexpr void operator+=(const value_type _) {  x += _; y += _; z += _; w += _; }
		constexpr void operator-=(const value_type _) {  x -= _; y -= _; z -= _; w -= _; }
		constexpr void operator/=(const value_type _) {  x /= _; y /= _; z /= _; w /= _; }
		constexpr void operator*=(const value_type _) {  x *= _; y *= _; z *= _; w *= _; }
		inline value_type* Data() { return &x; }
		inline const value_type* Data() const { return &x; }

		void draw_gui_control(const char* pID, float pColumnWidth, type pDefaults);

		VM_FUNC value_type MinComponent() { return math::Min(x, y, z, w); }
		VM_FUNC value_type MaxComponent() { return math::Max(x, y, z, w); }

		VM_FUNC type_float Dist(const type& _) const { return math::Dist(*this, _); }
		VM_FUNC type_float DistSqr(const type& _) const { return math::DistSqr(*this, _); }
		VM_FUNC type_float Length() const { return Dist(ZeroVector()); }
		VM_FUNC type Normalize() const { return this / Length(); }
		VM_FUNC bool AlmostEqual(const type& _, float tollerance) const { return math::is_equal(*this, _, tollerance); }

		[[nodiscard]] static
		const type& ZeroVector() { static type rval = type(0); return rval; }
		[[nodiscard]] static
		const type& OneVector() { static type rval = type(1); return rval; }
	private:
	};

#undef VM_FUNC

	template<typename T> using vec1 = vec<1, T>;
	template<typename T> using vec2 = vec<2, T>;
	template<typename T> using vec3 = vec<3, T>;
	template<typename T> using vec4 = vec<4, T>;

	using vec1 = vec1<float>;
	using vec2 = vec2<float>;
	using vec3 = vec3<float>;
	using vec4 = vec4<float>;

	using ivec1 = vec1<int>;
	using ivec2 = vec2<int>;
	using ivec3 = vec3<int>;
	using ivec4 = vec4<int>;

	using uvec1 = vec1<unsigned int>;
	using uvec2 = vec2<unsigned int>;
	using uvec3 = vec3<unsigned int>;
	using uvec4 = vec4<unsigned int>;
};