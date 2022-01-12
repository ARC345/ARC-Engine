#pragma once
#include "macros.h"
#include <iosfwd>
#include <string>

namespace ARC { namespace Core { class CApplication; } }

namespace ARC { class CEvent; }
#define OUT

using uint = unsigned int;
using uchar = unsigned char;
using ulong = unsigned long;

namespace ARC {
	namespace HPR {
		namespace Convert {
			template<typename T> inline T SecondsToMilliSeconds(T _Time) { return _Time * 1000; }
			template<typename T> inline T MilliSecondsToSeconds(T _Time) { return _Time / 1000; }
		}
		namespace IO {
			std::string ReadFile(const std::string& _Path);
			std::string ExtractFileNameFromPath(const std::string& _Path, bool _bRemoveExtention = true);
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
}