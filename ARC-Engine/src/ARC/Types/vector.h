#pragma once

#include "TybeBase.h"
#include <type_traits>
#include <initializer_list>
#include "ARC/Helpers/Math.h"
#include "spdlog/spdlog.h"

namespace ARC
{
	#define VM_FUNC [[nodiscard]] inline constexpr

	namespace Base
	{
		template <class T, size_t N>
		struct ARC_API TVec_Base : public TTypeBase {
		protected:
			TVec_Base() = default;
		public:
			using Super = TTypeBase;
			using value_type = T;
			using size_type = size_t;
			using type = TVec_Base<value_type, N>;

			value_type& operator[](size_type _)	{ return begin()[_]; };
			const value_type& operator[](size_type _) const	{ return begin()[_]; };
			
			VM_FUNC size_type size() { return N; }
			value_type* begin() { return (value_type*)this; }
			value_type* end() {	return begin() + N;	}

			const value_type* begin() const { return (value_type*)this; }
			const value_type* end() const { return begin() + N; }
			TString ToString() {
				TString x = fmt::format("{}", *begin()) ;
				for (size_t i = 1; i < N; i++)
				{
					x = fmt::format("{}, {}", x, (*this)[i]);
				}
				return fmt::format("[{}]", x);
			}
		};
	}
	template <size_t N, class T>
	class ARC_API TVec : public Base::TVec_Base<T, N>
	{
	public:
		using value_type = T;
		using Super = typename Base::TVec_Base<value_type, N>;
		using type = TVec<N, value_type>;
		using type_float = typename std::common_type<value_type, float>::type;
		using size_type = size_t;

		TVec() : mData() {};
		TVec(const value_type* _) : mData(_) {};
		TVec(const value_type& _) {	for (auto& iv : *this)	iv=_; };

		TVec(value_type Data[N]) : mData(Data) {}

		value_type& operator[](size_type _)	{ return mData[_]; };
		const value_type& operator[](size_type _) const { return mData[_]; };

		constexpr bool operator==(const type& _)
		{
			for (size_type i = 0; i < N; i++)
				if (Data()[i] != _.Data()[i])
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

		inline value_type* Data() { return mData; }
		inline const value_type* Data() const { return mData; }

	private:
		value_type mData[N];
	};

	template <class T>
	class ARC_API TVec<1, T> : public Base::TVec_Base<T, 1>
	{
	public:
		using Super = Base::TVec_Base<T, 1>;

		using value_type = T;
		using type = TVec<1, value_type>;
		using type_float = typename std::common_type<value_type, float>::type;
		using size_type = size_t;

		union { value_type x, r; };

		TVec() {}
		TVec(const value_type& _x) : x(_x){}

		void DrawGuiControl(const char* ID, float pColumnWidth);

		VM_FUNC type_float Dist(const type& _) const { return SMath::Abs(_-*this); }
		VM_FUNC type_float Length() const { return *this; }
		VM_FUNC type Normalize() const { return this / Length(); }
		VM_FUNC bool AlmostEqual(const type& _, float _Tollerance) const { return SMath::IsEqual(this, _, _Tollerance); }

		[[nodiscard]] static
		const type& ZeroVector() { static type rval = type(0); return rval; }
		[[nodiscard]] static
		const type& OneVector() { static type rval = type(1); return rval; }
	};

	template <class T>
	class ARC_API TVec<2, T> : public Base::TVec_Base<T, 2>
	{
		ARC_TYPE();
		
	public:
		
		using Super = Base::TVec_Base<T, 2>;

		using value_type = T;
		using type = TVec<2, value_type>;
		using type_float = typename std::common_type<value_type, float>::type;
		using size_type = size_t;

		union { value_type x, r; };
		union { value_type y, g; };
		
		TVec() {}
		TVec(const value_type& _x, const value_type& _y) : x(_x), y(_y) {}
		TVec(const value_type& _) : x(_), y(_) {}

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

		inline value_type* Data() { return &x; }
		inline const value_type* Data() const { return &x; }

		void DrawGuiControl(const char* pID, float pColumnWidth, type pDefaults);
		
		VM_FUNC value_type MinComponent() const { return SMath::Min(x, y); }
		VM_FUNC value_type MaxComponent() const { return SMath::Max(x, y); }

		VM_FUNC bool IsWithinBounds(const type& pMin, const type& pMax)
		{
			return SMath::IsInRange(x, pMin.x, pMax.x) && SMath::IsInRange(y, pMin.y, pMax.y);
		}

		VM_FUNC type_float Dist(const type& _) const { return SMath::Dist(*this, _); }
		VM_FUNC type_float DistSqr(const type& _) const { return SMath::DistSqr(*this, _); }
		VM_FUNC type_float Length() const { return Dist(ZeroVector()); }
		VM_FUNC type Normalize() const { return this / Length(); }
		VM_FUNC bool AlmostEqual(const type& _, float _Tollerance) const { return SMath::IsEqual(*this, _, _Tollerance); }
		[[nodiscard]] static
		const type& ZeroVector() { static type rval = type(0); return rval; }
		[[nodiscard]] static
		const type& OneVector() { static type rval = type(1); return rval; }
	private:
	};
	
	template <class T>
	class ARC_API TVec<3, T> : public Base::TVec_Base<T, 3>
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

		TVec() {}
		TVec(const value_type& _x, const value_type& _y, const value_type& _z) : x(_x), y(_y), z(_z) {}
		TVec(const value_type _) : x(_), y(_), z(_) {}

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

		void DrawGuiControl(const char* pID, float pColumnWidth, type pDefaults);

		VM_FUNC value_type MinComponent() const { return SMath::Min(x, y, z); }
		VM_FUNC value_type MaxComponent() const { return SMath::Max(x, y, z); }

		VM_FUNC type_float Dist(const type& _) const { return SMath::Dist(*this, _); }
		VM_FUNC type_float DistSqr(const type& _) const { return SMath::DistSqr(*this, _); }
		VM_FUNC type_float Length() const { return Dist(ZeroVector()); }

		VM_FUNC type Cross(const type& _1, const type& _2) {
			return (_1.y * _2.z - _2.y * _1.z,
				_1.z * _2.x - _2.z * _1.x,
				_1.x * _2.y - _2.x * _1.y);
		}

		VM_FUNC type Normalize() const { return *this / Length(); }
		VM_FUNC bool AlmostEqual(const type& _, float _Tollerance) const { return SMath::IsEqual(*this, _, _Tollerance); }

		VM_FUNC type Mask(type _) { return *this * _; }
		[[nodiscard]] static
		const type& ZeroVector() { static type rval = type(0); return rval; }
		[[nodiscard]] static
		const type& OneVector() { static type rval = type(1); return rval; }
	private:
	};

	template <class T>
	class ARC_API TVec<4, T> : public Base::TVec_Base<T, 4>
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

		TVec() {}
		TVec(const value_type& _x, const value_type& _y, const value_type& _z, const value_type& _w) : x(_x), y(_y), z(_z), w(_w) {}
		TVec(const value_type _) : x(_), y(_), z(_), w(_) {}

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

		void DrawGuiControl(const char* pID, float pColumnWidth, type pDefaults);

		VM_FUNC value_type MinComponent() { return SMath::Min(x, y, z, w); }
		VM_FUNC value_type MaxComponent() { return SMath::Max(x, y, z, w); }

		VM_FUNC type_float Dist(const type& _) const { return SMath::Dist(*this, _); }
		VM_FUNC type_float DistSqr(const type& _) const { return SMath::DistSqr(*this, _); }
		VM_FUNC type_float Length() const { return Dist(ZeroVector()); }
		VM_FUNC type Normalize() const { return this / Length(); }
		VM_FUNC bool AlmostEqual(const type& _, float _Tollerance) const { return SMath::IsEqual(*this, _, _Tollerance); }

		[[nodiscard]] static
		const type& ZeroVector() { static type rval = type(0); return rval; }
		[[nodiscard]] static
		const type& OneVector() { static type rval = type(1); return rval; }
	private:
	};

#undef VM_FUNC

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