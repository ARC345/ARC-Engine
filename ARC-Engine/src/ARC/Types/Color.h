#pragma once

#include "Vector.h"

namespace ARC {
	class FColor : public FVec4
	{
	public:
		FColor() : FVec4(FVec4::ZeroVector) {}
		FColor(const FVec4& _) : FVec4(_) {};
		FColor(float _r, float _g, float _b, float _a) : FVec4(_r, _g, _b, _a) {};

		static FColor White;
		static FColor Red;
		static FColor Green;
		static FColor Blue;
		static FColor Yellow;
		static FColor Black;
	};
}