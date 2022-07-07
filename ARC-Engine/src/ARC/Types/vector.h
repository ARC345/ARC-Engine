#pragma once

#include "TybeBase.h"
#include <type_traits>
#include <initializer_list>
#include "String.h"

namespace ARC
{
#define VM_FUNC [[nodiscard]] inline constexpr

	namespace Base
	{
		template <typename T, size_t N>
		struct ARC_API TVec_Base : public TTypeBase {
		protected:
			TVec_Base() = default;
		public:
			using Super = TTypeBase;
			using value_type = T;
			using size_type = size_t;
			using type = TVec_Base<value_type, N>;

			value_type& operator[](size_type _)
			{
				return begin()[_];
			};
			const value_type& operator[](size_type _)const
			{
				return begin()[_];
			};

			VM_FUNC size_type Size() {
				return N;
			}

			value_type* begin() { return (value_type*)this; }
			value_type* end() {	return begin() + N - 1;	}

			const value_type* begin() const { return (value_type*)this; }
			const value_type* end() const { return begin() + N - 1; }
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
	template <size_t N, typename T>
	class ARC_API TVec : public Base::TVec_Base<T, N>
	{
	public:
		using value_type = T;
		using Super = Base::TVec_Base<value_type, N>;
		using type = TVec<N, value_type>;
		using type_float = typename std::common_type<value_type, float>::type;
		using size_type = size_t;

		type() : m_Data() {};
		type(const value_type* _) : m_Data(_)
		{
		};
		type(const value_type _)
		{
			for (auto& iv : *this)
				iv=x;
		};

		type(value_type Data[N])
			: m_Data(Data)
		{}

		value_type& operator[](size_type _)
		{
			return m_Data[_];
		};
		const value_type& operator[](size_type _) const
		{
			return m_Data[_];
		};

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
		constexpr void operator+=(const value_type _) { iv += type(_); }
		constexpr void operator-=(const value_type _) { iv -= type(_); }
		constexpr void operator*=(const value_type _) { iv *= type(_); }
		constexpr void operator/=(const value_type _) { iv /= type(_); }

		constexpr type operator+(const value_type _) const { return *this+type(_); }
		constexpr type operator-(const value_type _) const { return *this-type(_); }
		constexpr type operator*(const value_type _) const { return *this*type(_); }
		constexpr type operator/(const value_type _) const { return *this/type(_); }

		inline value_type* Data() { return m_Data; }
		inline const value_type* Data() const { return m_Data; }

	private:
		value_type m_Data[N];
	};

	template <typename T>
	class ARC_API TVec<1, T> : public Base::TVec_Base<T, 1>
	{
	public:
		using Super = Base::TVec_Base<T, 1>;

		using value_type = T;
		using type = TVec<1, value_type>;
		using type_float = typename std::common_type<value_type, float>::type;
		using size_type = size_t;

		union { value_type x, r; };

		type() {}
		type(const value_type& _x) : x(_x){}

		void DrawGuiControl(const char* ID, float pColumnWidth);

		VM_FUNC type_float Dist(const type& _) const { return Math::Dist(*this, _); }
		VM_FUNC type_float DistSqr(const type& _) const { return Math::DistSqr(*this, _); }
		VM_FUNC type_float Length() const { return Dist(ZeroVector); }
		VM_FUNC type Normalize() const { return this / Length(); }
		VM_FUNC bool AlmostEqual(const type& _, float _Tollerance) const { return Math::AlmostEqual(this, _, _Tollerance); }
	};

	template <typename T>
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
		
		type() {}
		type(const value_type& _x, const value_type& _y) : x(_x), y(_y) {}
		type(const value_type& _) : x(_), y(_) {}

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
		
		//M_FUNC static bool AreCollinear(const type& _1, const type& _2, value_type _Tollerance) { return Math::Abs(_1.x - _2.x) <= _Tollerance && Math::Abs(_1.y - _2.y) <= _Tollerance; }
		//M_FUNC static bool AreCollinitial(const type& _1, const type& _2, value_type _Tollerance) { return Math::Abs(_1.x - _2.x) <= _Tollerance && Math::Abs(_1.y - _2.y) <= _Tollerance; }
		
		VM_FUNC value_type MinComponent() const { return Math::Min(x, y); }
		VM_FUNC value_type MaxComponent() const { return Math::Max(x, y); }

		VM_FUNC type_float Dist(const type& _) const { return Math::Dist(*this, _); }
		VM_FUNC type_float DistSqr(const type& _) const { return Math::DistSqr(*this, _); }
		VM_FUNC type_float Length() const { return Dist(ZeroVector); }
		VM_FUNC type Normalize() const { return this / Length(); }
		VM_FUNC bool AlmostEqual(const type& _, float _Tollerance) const { return Math::AlmostEqual(*this, _, _Tollerance); }
	public:
		static type ZeroVector;
		static type OneVector;
	private:
	};
	
	template <typename T>
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

		type() {}
		type(const value_type& _x, const value_type& _y, const value_type& _z) : x(_x), y(_y), z(_z) {}
		type(const value_type _) : x(_), y(_), z(_) {}

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
 		void DrawGui(float pColumnWidth) const;

		VM_FUNC value_type MinComponent() const { return Math::Min(x, y, z); }
		VM_FUNC value_type MaxComponent() const { return Math::Max(x, y, z); }

		VM_FUNC type_float Dist(const type& _) const { return Math::Dist(*this, _); }
		VM_FUNC type_float DistSqr(const type& _) const { return Math::DistSqr(*this, _); }
		VM_FUNC type_float Length() const { return Dist(ZeroVector); }

		VM_FUNC type Normalize() const { return *this / Length(); }
		VM_FUNC bool AlmostEqual(const type& _, float _Tollerance) const { return Math::AlmostEqual(*this, _, _Tollerance); }

		VM_FUNC type Mask(type _) { return *this*_; }

		template<typename T> T To() const { return HPR::Conv<T>(*this); }
	public:
		static type ZeroVector;
		static type OneVector;
	private:
	};

	template <typename T>
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

		type() {}
		type(const value_type& _x, const value_type& _y, const value_type& _z, const value_type& _w) : x(_x), y(_y), z(_z), w(_w) {}
		type(const value_type _) : x(_), y(_), z(_), w(_) {}

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

		VM_FUNC value_type MinComponent() { return Math::Min(x, y, z, w); }
		VM_FUNC value_type MaxComponent() { return Math::Max(x, y, z, w); }

		VM_FUNC type_float Dist(const type& _) const { return Math::Dist(*this, _); }
		VM_FUNC type_float DistSqr(const type& _) const { return Math::DistSqr(*this, _); }
		VM_FUNC type_float Length() const { return Dist(ZeroVector); }
		VM_FUNC type Normalize() const { return this / Length(); }
		VM_FUNC bool AlmostEqual(const type& _, float _Tollerance) const { return Math::AlmostEqual(*this, _, _Tollerance); }
	public:
		static type ZeroVector;
		static type OneVector;
	private:
	};

#undef VM_FUNC

	template<typename T> using TVec1 = TVec<1, T>;
	template<typename T> using TVec2 = TVec<2, T>;
	template<typename T> using TVec3 = TVec<3, T>;
	template<typename T> using TVec4 = TVec<4, T>;

	template<typename T> TVec2<T> TVec2<T>::ZeroVector = TVec2<T>(0, 0);
	template<typename T> TVec3<T> TVec3<T>::ZeroVector = TVec3<T>(0, 0, 0);
	template<typename T> TVec4<T> TVec4<T>::ZeroVector = TVec4<T>(0, 0, 0, 0);
	
	template<typename T> TVec2<T> TVec2<T>::OneVector = TVec2<T>(1, 1);
	template<typename T> TVec3<T> TVec3<T>::OneVector = TVec3<T>(1, 1, 1);
	template<typename T> TVec4<T> TVec4<T>::OneVector = TVec4<T>(1, 1, 1, 1);

	using FVec1 = TVec1<float>;
	using FVec2 = TVec2<float>;
	using FVec3 = TVec3<float>;
	using FVec4 = TVec4<float>;
};