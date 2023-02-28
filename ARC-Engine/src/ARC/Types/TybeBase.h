#pragma once
#include <string>
#include "ARC\Core\Macros.h"

#define ARC_TYPE() // public: template<typename __T__> __T__ To() const { return HPR::template Conv<__T__>(*this); } private:

namespace ARC
{
	struct ARC_API TTypeBase
	{
	protected:
		TTypeBase() {};
	};

	using TString = std::string;
	using TName = const char*; // very temp

	using TUInt = unsigned int;
	using TUChar = unsigned char;
	using TULong = unsigned long;

	using TUInt8 = uint8_t;
	using TUInt16 = uint16_t;
	using TUInt32 = uint32_t;
	using TUInt64 = uint64_t;

	using TFloat = float;
}