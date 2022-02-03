#pragma once

#include "TybeBase.h"
#include <type_traits>
#include "../Helpers/Helpers.h"
#include <string>
#include "Math.h"

namespace ARC
{
	template <size_t N, typename T = float>
	class TVecX;
	
	template <size_t N, typename T = float>
	std::ostream& operator<<(std::ostream& os, const TVecX<N, T>& e)
	{
		os << "[";
		for (uint a = 0; a < N; a++)
			os << e[a] << ((a != N - 1) ? ", " : "]");
		return os;
	}

	namespace Base
	{
		struct ARC_API TVec_Base : public TTypeBase{};
	}

	template <size_t N, typename T>
	class ARC_API TVecX : public Base::TVec_Base
	{
		
	public:
		TVecX() : m_Data() {};
		TVecX(const TVecX<N, T>& x)
		{
			for (uint a=0;a<N;a++ )
				m_Data[a] = x[a];
		};
		TVecX(const T* x)
		{
			for (uint a=0;a<=N;a++ )
			{
				m_Data[a]=x[a];
			};
		};

		TVecX(T Data[N])
			: m_Data(Data)
		{}
		T& operator[](size_t x)
		{
			return m_Data[x];
		};
		const T& operator[](size_t x)const
		{
			return m_Data[x];
		};

		constexpr bool operator==(const TVecX<N, T>& x)
		{
			for (size_t i = 0; i < N; i++)
			{
				if (this->Data()[i] != x.Data()[i]) return false;
			}
			return true;
		}
		constexpr TVecX<N, T> operator+(const TVecX<N, T>& x)
		{
			TVecX<N, T> rval;
			for (size_t i = 0; i < N; i++)
			{
				rval[i] = this->Data()[i] + x.Data()[i];
			}
			return rval;
		}
		constexpr TVecX<N, T> operator-(const TVecX<N, T>& x)
		{
			TVecX<N, T> rval;
			for (size_t i = 0; i < N; i++)
			{
				rval[i] = this->Data()[i] - x.Data()[i];
			}
			return rval;
		}
		constexpr TVecX<N, T> operator*(const TVecX<N, T>& x)
		{
			TVecX<N, T> rval;
			for (size_t i = 0; i < N; i++)
			{
				rval[i] = this->Data()[i] - x.Data()[i];
			}
			return rval;
		}
		constexpr TVecX<N, T> operator/(const TVecX<N, T>& x)
		{
			TVecX<N, T> rval;
			for (size_t i = 0; i < N; i++)
			{
				rval[i] = this->Data()[i] / x.Data()[i];
			}
			return rval;
		}

		inline T* Data() { return m_Data; }
		inline const T* Data() const { return m_Data; }

		T* begin() { return &m_Data[0]; }
		T* end() { return &m_Data[GetSize()]; }

		virtual constexpr size_t GetSize() const { return N; }
		friend std::ostream& operator<< <>(std::ostream& os, const TVecX<N,T>& e);

	public:
		
	private:
		T m_Data[N];
	};

	template <typename T = float>
	class ARC_API TVec2 : public TVecX<2,T>
	{
	public:
		using Super = TVecX<2, T>;

		TVec2() :
			r(Data()[0]), g(Data()[1]),
			x(Data()[0]), y(Data()[1])
		{}
		TVec2(const T& tx, const T& ty) :
			r(Data()[0]), g(Data()[1]),
			x(Data()[0]), y(Data()[1])
		{
			this->Set(tx, ty);
		}

		T& r, & g;
		T& x, & y;

		[[nodiscard]] static inline constexpr float Dist(const TVec2<T>& _1, const TVec2<T>& _2) {
			return Math::Sqrt(Math::DistSqr(_1, _2));
		}
		
		[[nodiscard]] static inline constexpr float DistSqr(const TVec2<T>& _1, const TVec2<T>& _2) {
			return Math::Sqr(_1.x - _2.x) + Math::Sqr(_1.y - _2.y);
		}

		inline void Set(const T& tx, const T& ty) { x = tx; y = ty; };

	public:
		static TVec2<T> ZeroVector;
	private:
	};

	template <typename T = float>
	class ARC_API TVec3 : public TVecX<3,T>
	{
	public:
		using Super = TVecX<3, T>;

		TVec3() :
			r(Data()[0]), g(Data()[1]), b(Data()[2]),
			x(Data()[0]), y(Data()[1]), z(Data()[2])
		{}
		TVec3(const T& tx, const T& ty, const T& tz) :
			r(Data()[0]), g(Data()[1]), b(Data()[2]),
			x(Data()[0]), y(Data()[1]), z(Data()[2])
		{
			this->Set(tx, ty, tz);
		}

		T& r, & g, & b;
		T& x, & y, & z;

		[[nodiscard]] inline constexpr float Dist(const TVec2<T>& _1, const TVec2<T>& _2) {
			return Math::Sqrt(Math::DistSqr(_1, _2));
		}

		[[nodiscard]] inline constexpr float DistSqr(const TVec2<T>& _1, const TVec2<T>& _2) {
			return Math::Sqr(_1.x - _2.x) + Math::Sqr(_1.y - _2.y) + Math::Sqr(_1.z - _2.z);
		}

		inline void Set(const T& tx, const T& ty, const T& tz) { *x = tx; *y = ty; *z = tz;};

	public:
		static TVec3<T> ZeroVector;

	};
	template <typename T = float>
	class ARC_API TVec4 : public TVecX<4, T>
	{
	public:
		using Super = TVecX<4, T>;
		
		TVec4() {}
		TVec4(const T& tw, const T& tx, const T& ty, const T& tz) :
			r(Data()[0]), g(Data()[1]), b(Data()[2]), a(Data()[3]),
			x(Data()[0]), y(Data()[1]), z(Data()[2]), w(Data()[3])
		{
			this->Set(tw, tx, ty, tz);
		}

		T& r, & g, & b, & a;
		T& x, & y, & z, & w; 

		inline void Set(const T& tx, const T& ty, const T& tz, const T& tw) {
			x = tx; y = ty; z = tz;  w = tw;
		};

	public:
		static TVec4<T> ZeroVector;
	};

	template<typename T> TVec2<T> TVec2<T>::ZeroVector = {0, 0};
	template<typename T> TVec3<T> TVec3<T>::ZeroVector = {0, 0, 0};
	template<typename T> TVec4<T> TVec4<T>::ZeroVector = {0, 0, 0, 0};

};