#pragma once
#include "macros.h"
#include <iosfwd>

namespace ARC { namespace Core { class CApplication; } }

namespace ARC { class CEvent; }
#define OUT

using uint = unsigned int;
using uchar = unsigned char;
using ulong = unsigned long;

namespace ARC {
	namespace Convert {
		template<typename T> inline T SecondsToMilliSeconds(T _Time) { return _Time * 1000; }
		template<typename T> inline T MilliSecondsToSeconds(T _Time) { return _Time / 1000; }
	}

	namespace BIT {
// 	template<typename T>
// 		inline static bool Contains(uint BitField, const T& flag) {
// 			static_assert(std::is_enum(T));
// 			return BitField & T;
// 		}
 	}
	namespace INP {

	}
}