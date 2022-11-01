#pragma once
#include <string>
#include "ARC\Core\Macros.h"

namespace ARC
{
	#define ARC_TYPE() public: template<typename __T__> __T__ To() const { return HPR::Conv<__T__>(*this); } private:

	struct ARC_API TTypeBase
	{
	protected:
		TTypeBase() {};
	};

	using TString = std::string;
}