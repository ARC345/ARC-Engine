#pragma once
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned long ulong;

namespace ARC::Math
{
	struct FVec
	{
	FVec(){};
	protected:
		virtual constexpr size_t GetSize() const =0;
	};

	template <size_t N, typename T = float>
	struct FVecX : public FVec
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
// 
// 		FVecX<N, T> operator +(const FVecX<N,T>& x)
// 		{
// 			FVecX<N, T> rval;
// 			for (uint _x=0; x<=N; _x++)
// 			{
// 				rval[_x] = this[_x] + x[_x];
// 			}
// 			return rval;
// 		};
// 		float* operator +(const float*& x)
// 		{
// 			float* rval;
// 			for (uint _x=0; x<=N; _x++)
// 			{
// 				rval[_x] = this[_x] + x[_x];
// 			}
// 			return rval;
// 		};
// 		void operator +=(const FVecX<N,T>& x)
// 		{
// 			for (uint _x=0; _x<=N; _x++)
// 			{
// 				this[_x] += x[_x];
// 			}
// 		};
// 		void operator +=(const float*& x)
// 		{
// 			for (uint _x = 0; x <= N; _x++)
// 			{
// 				this[_x] += x[_x];
// 			}
// 		};

		__forceinline T* Data() {return m_Data;}
		__forceinline const T* Data() const {return m_Data;}

		virtual constexpr size_t GetSize() const override {return N;}		

	private:
		T m_Data[N];

	};

	template <typename T = float>
	struct FVec2 : public FVecX<2,T>
	{
		using Super = FVecX<2,T>;

		FVec2() {}
		FVec2(const T& tx, const T& ty)
		{
			this->Set(tx, ty);
		}
		__forceinline T& x() {return Data()[0];};
		__forceinline T& y() {return Data()[1];};

		__forceinline T* Data() {return Super::Data();};
		__forceinline const T* Data() const {return Super::Data();};


		inline void Set(const T& tx, const T& ty) { x() = tx; y() = ty; };
	};

	template <typename T = float>
	struct FVec3 : public FVecX<3,T>
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
		__forceinline T* Data() { return Super::Data(); };
		__forceinline const T* Data() const { return Super::Data(); };

		inline void Set(const T& tx, const T& ty, const T& tz) { x() = tx; y() = ty; z() = tz;};
		virtual constexpr size_t GetSize() const override {return 3;}
	};
	template <typename T = float>
	struct FVec4 : public FVecX<4, T>
	{
		using Super = FVecX<4, T>;
		
		FVec4() {}
		FVec4(const T& tw, const T& tx, const T& ty, const T& tz)
		{
			this->Set(tw, tx, ty, tz);
		}

		T& w() { return Data()[0]; };
		T& x() { return Data()[1]; };
		T& y() { return Data()[2]; };
		T& z() { return Data()[3]; };
		
		T& r() { return w(); };
		T& g() { return x(); };
		T& b() { return y(); };
		T& a() { return z(); };

		__forceinline T* Data() { return Super::Data(); };

		inline void Set(const T& tw, const T& tx, const T& ty, const T& tz) { w()=tw, x() = tx; y() = ty; z() = tz;};
		virtual constexpr size_t GetSize() const override {return 4;}
	};
};