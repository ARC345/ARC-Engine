#pragma once

#include <random>
#include <limits>
#include <stdint.h>
#include <algorithm>
#include "ARC\Types\Vector.h"
#include "ARC\Types\Transform2D.h"
#include "Math.h"

namespace ARC { namespace Core { class CApplication; } }

namespace ARC { class CEvent; }
#define OUT

using uint = unsigned int;
using uchar = unsigned char;
using ulong = unsigned long;

struct CounterId {};

enum EQuadCorner {
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight
};

enum class ERandom {
	Normal,
	None
};

namespace ARC {
	namespace HPR {
		template<typename To, typename From>
		To Conv(const From& _);

		template<typename T, typename U> static constexpr size_t GetDataMemberOffset(U T::* member)
		{
			return (char*)&((T*)nullptr->*member) - (char*)nullptr;
		}

		namespace IO {
			[[nodiscard]] std::string ReadFile(const std::string& _Path);
			[[nodiscard]] std::string ExtractFileNameFromPath(const std::string& _Path, bool _bRemoveExtention = true);
		}

		namespace QUAD {
			template<EQuadCorner T1> FVec2 GetCorner(const FTransform2D& Trans) { 
				using namespace Math;

				switch (T1)
				{
				case TopRight: return { 
					Trans.Location.x + ((Trans.Scale.x / 2) * Cos(Trans.Rotation)) - ((Trans.Scale.y / 2) * Sin(Trans.Rotation)),
					Trans.Location.y + ((Trans.Scale.x / 2) * Sin(Trans.Rotation)) + ((Trans.Scale.y / 2) * Cos(Trans.Rotation)) };
				case BottomLeft: return {
					Trans.Location.x - ((Trans.Scale.x / 2) * Cos(Trans.Rotation)) + ((Trans.Scale.y / 2) * Sin(Trans.Rotation)),
					Trans.Location.y - ((Trans.Scale.x / 2) * Sin(Trans.Rotation)) - ((Trans.Scale.y / 2) * Cos(Trans.Rotation)) };
				case TopLeft: return {
					Trans.Location.x - ((Trans.Scale.x / 2) * Cos(Trans.Rotation)) - ((Trans.Scale.y / 2) * Sin(Trans.Rotation)),
					Trans.Location.y - ((Trans.Scale.x / 2) * Sin(Trans.Rotation)) + ((Trans.Scale.y / 2) * Cos(Trans.Rotation)) };
				case BottomRight: return { 
					Trans.Location.x + ((Trans.Scale.x / 2) * Cos(Trans.Rotation)) + ((Trans.Scale.y / 2) * Sin(Trans.Rotation)),
					Trans.Location.y + ((Trans.Scale.x / 2) * Sin(Trans.Rotation)) - ((Trans.Scale.y / 2) * Cos(Trans.Rotation)) };
				default:
					return { 0, 0 };
				}
			}
			
		}

		class Random
		{
		public:
			using randclass = std::mt19937;

			static void Init()
			{
				s_RandomEngine32.seed(std::random_device()());
			}
			template<typename T>
			static void Init(const T* _Seed)
			{
				if constexpr(_Seed == nullptr)
					Init();
				else
					s_RandomEngine32.seed(*_Seed);
			}

			template<typename T>
			static T Get() {
				return (T)s_Distribution(s_RandomEngine32);
			}
			template<ERandom T>
			static auto Get() {
				if constexpr (T == ERandom::Normal) {
					return (float)s_Distribution(s_RandomEngine32) / (float)randclass::max();
				}
				else 
					return uint32_t(10);
			}
			
			template<typename T>
			static T Get(T _Min, T _Max) {
				return _Min + ((T)s_Distribution(s_RandomEngine32) / ((T)randclass::max() / (_Max - _Min)));
			}
			
			static inline float Normal() { return Random::Get<ERandom::Normal>(); }

			static inline float Float() { return Random::Get<float>(); }
			static inline float Float(float _Min, float _Max) { return Random::Get(_Min, _Max); }

			static inline int Int() { return Random::Get<int>(); }
			static inline int Int(int _Min, int _Max) { return Random::Get(_Min, _Max); }
			
		private:
			static std::mt19937 s_RandomEngine32;
			static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
		};

		template<typename T>
		class CustomCounter
		{
			template<int N>
			struct Flag { friend constexpr int setFlag(Flag<N>); };

			template<int N>
			struct Writer
			{
				friend constexpr int setFlag(Flag<N>) { return 0; }
			};

			template<int N>
			static constexpr int reader(float, Flag<N>) { return N; }

			template<int N, int = setFlag(Flag<N>{}) >
			static constexpr int reader(int, Flag<N>, int value = reader(0, Flag<N + 1>{}))
			{
				return value;
			}

		public:
			template<int N = reader(0, Flag<0>{}), int = sizeof(Writer<N>) >
			static constexpr int Next() { return N; }
		};
		struct CounterId_Default : CounterId {};
		using Counter = CustomCounter<CounterId_Default>;

		template<typename T>
		constexpr const T& Zero() { 
			static T _;
			return _;
		};
	}
}