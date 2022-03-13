#pragma once

#include <random>
#include <limits>
#include <stdint.h>
#include <algorithm>
#include "..\Types\vector.h"
#include "..\Types\Transform2D.h"
#include "Math.h"

namespace ARC { namespace Core { class CApplication; } }

namespace ARC { class CEvent; }
#define OUT

using uint = unsigned int;
using uchar = unsigned char;
using ulong = unsigned long;

enum EQuadCorner {
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight
};

namespace ARC {
	namespace HPR {

		template<typename T, typename U> static constexpr size_t GetOffset(U T::* member)
		{
			return (char*)&((T*)nullptr->*member) - (char*)nullptr;
		}

		namespace IO {
			[[nodiscard]] std::string ReadFile(const std::string& _Path);
			[[nodiscard]] std::string ExtractFileNameFromPath(const std::string& _Path, bool _bRemoveExtention = true);
		}

		namespace QUAD {
			template<EQuadCorner T1> FVec2 GetScaledCorner(const FTransform2D& Trans) { 
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
			static void Init()
			{
				s_RandomEngine.seed(std::random_device()());
			}


			static float Float()
			{
				return (float)s_Distribution(s_RandomEngine) / (float)0xFFFFFFFF;
			}
			
			static float Float(float _Min, float _Max)
			{
				return _Min+((float)s_Distribution(s_RandomEngine) / (float)(0xFFFFFFFF/(_Max-_Min)));
			}
			
		private:
			static std::mt19937 s_RandomEngine;
			static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
		};

	//	namespace BIT {
	// 	template<typename T>
	// 		inline static bool Contains(uint BitField, const T& flag) {
	// 			static_assert(std::is_enum(T));
	// 			return BitField & T;
	// 		}
 	//	}
		namespace INP {

		}
	}
}