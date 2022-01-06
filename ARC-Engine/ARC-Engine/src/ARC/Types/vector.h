#pragma once

#include "TybeBase.h"
#include <type_traits>
#include "../Helpers/Helpers.h"
#include <string>

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

		__forceinline T* Data() {return m_Data;}
		__forceinline const T* Data() const {return m_Data;}

		virtual constexpr size_t GetSize() const { return N; }
		friend std::ostream& operator<< <>(std::ostream& os, const TVecX<N,T>& e);

	private:
		T m_Data[N];
		size_t m_Size = N;

	};

	template <typename T = float>
	class ARC_API TVec2 : public TVecX<2,T>
	{
	public:
		using Super = TVecX<2, T>;

		TVec2() {}
		TVec2(const T& tx, const T& ty)
		{
			this->Set(tx, ty);
		}
		T& x() { return Data()[0]; };
		T& y() { return Data()[1]; };

		inline void Set(const T& tx, const T& ty) { x() = tx; y() = ty; };
	};

	template <typename T = float>
	class ARC_API TVec3 : public TVecX<3,T>
	{
	public:
		using Super = TVecX<3, T>;

		TVec3(){}
		TVec3(const T& tx, const T& ty, const T& tz)
		{
			this->Set(tx, ty, tz);
		}

		T* x = &Data()[0];
		T* y = &Data()[1];
		T* z = &Data()[2];

		inline void Set(const T& tx, const T& ty, const T& tz) { *x = tx; *y = ty; *z = tz;};
	};
	template <typename T = float>
	class ARC_API TVec4 : public TVecX<4, T>
	{
	public:
		using Super = TVecX<4, T>;
		
		TVec4() {}
		TVec4(const T& tw, const T& tx, const T& ty, const T& tz)
		{
			this->Set(tw, tx, ty, tz);
		}

		T& w() const { return Data()[0]; };
		T& x() const { return Data()[1]; };
		T& y() const { return Data()[2]; };
		T& z() const { return Data()[3]; };
		
		T& r() const { return w(); };
		T& g() const { return x(); };
		T& b() const { return y(); };
		T& a() const { return z(); };

		inline void Set(const T& tw, const T& tx, const T& ty, const T& tz) { w()=tw, x() = tx; y() = ty; z() = tz;};
	};

};