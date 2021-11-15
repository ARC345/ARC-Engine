#pragma once
#include "container.h"
#include <type_traits>
#include "../Helpers/Helpers.h"

namespace ARC
{
	namespace Base
	{
		struct ARC_API FVec_Base : public FContainer{};
	}

	template <size_t N, typename T = float>
	struct ARC_API FVecX : public Base::FVec_Base
	{
		FVecX() : m_Data(){};
		FVecX(const FVecX<N, T>& x)
		{
			for (uint a=0;a<=N;a++ )
			{
				m_Data[a]=x[a];
			};
		};
		FVecX(const T* x)
		{
			for (uint a=0;a<=N;a++ )
			{
				m_Data[a]=x[a];
			};
		};
		FVecX(T Data[N])
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
		virtual __forceinline const char* GetName() const override {
			return "FVecX";
		}

	private:
		T m_Data[N];
		size_t m_Size = N;

	};

	template <typename T = float>
	struct ARC_API FVec2 : public FVecX<2,T>
	{
		using Super = FVecX<2,T>;

		FVec2() {}
		FVec2(const T& tx, const T& ty)
		{
			this->Set(tx, ty);
		}
		T& const x()  { return Data()[0]; };
		T& const y()  { return Data()[1]; };

		inline void Set(const T& tx, const T& ty) { x() = tx; y() = ty; };
	};

	template <typename T = float>
	struct ARC_API FVec3 : public FVecX<3,T>
	{
		using Super = FVecX<3, T>;

		FVec3(){}
		FVec3(const T& tx, const T& ty, const T& tz)
		{
			this->Set(tx, ty, tz);
		}

		T& x() { return Data()[0]; };
		T& y() { return Data()[1]; };
		T& z() { return Data()[2]; };

		inline void Set(const T& tx, const T& ty, const T& tz) { x() = tx; y() = ty; z() = tz;};
	};
	template <typename T = float>
	struct ARC_API FVec4 : public FVecX<4, T>
	{
		using Super = FVecX<4, T>;
		
		FVec4() {}
		FVec4(const T& tw, const T& tx, const T& ty, const T& tz)
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