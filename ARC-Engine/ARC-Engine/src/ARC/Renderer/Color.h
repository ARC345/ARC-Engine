#pragma once

#include "ARC/Types/vector.h"

namespace ARC {
	class CColor : public FVec4
	{
	public:
		CColor() : FVec4(FVec4::ZeroVector) {}
		CColor(const FVec4& _) : FVec4(_) {};
		CColor(float _r, float _g, float _b, float _a) : FVec4(_r, _g, _b, _a) {};

		static CColor White;
		static CColor Red;
		static CColor Green;
		static CColor Blue;
		static CColor Yellow;
		static CColor Black;
	};
}