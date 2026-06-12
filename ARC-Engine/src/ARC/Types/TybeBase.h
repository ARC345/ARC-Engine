#pragma once
#include <string>
#include "ARC\Core\Macros.h"

#define ARC_TYPE() // public: template<typename __T__> __T__ To() const { return HPR::template Conv<__T__>(*this); } private:

namespace arc
{
	struct ARC_API type_base
	{
	protected:
		type_base() {};
	};

	using string = std::string;
	using name = const char*; // very temp

	using uint = unsigned int;
	using uchar = unsigned char;
	using ulong = unsigned long;

	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;

	using float32 = float;
}