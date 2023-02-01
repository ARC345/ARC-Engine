#pragma once

#include "Vector.h"

namespace ARC {
	class FColor4 : public FVec4
	{
	public:
		FColor4() : FVec4(FVec4::ZeroVector()) {}
		FColor4(const FVec4& _) : FVec4(_) {};
		FColor4(float _r, float _g, float _b, float _a) : FVec4(_r, _g, _b, _a) {};

		[[nodiscard]] static
		const FColor4& White() { static FColor4 rval = FColor4(1.f); return rval; }
		[[nodiscard]] static
		const FColor4& Red() { static FColor4 rval = FColor4(1.f, 0.f, 0.f, 1.f); return rval; }
		[[nodiscard]] static
		const FColor4& Green() { static FColor4 rval = FColor4(0.f, 1.f, 0.f, 1.f); return rval; }
		[[nodiscard]] static
		const FColor4& Blue() { static FColor4 rval = FColor4(0.f, 0.f, 1.f, 1.f); return rval; }
		[[nodiscard]] static
		const FColor4& Yellow() { static FColor4 rval = FColor4(1.f, 1.f, 0.f, 1.f); return rval; }
		[[nodiscard]] static
		const FColor4& Black() { static FColor4 rval = FColor4(0.f, 0.f, 0.f, 1.f); return rval; }
	};
}