#pragma once

#include "ARC/Types/vector.h"

namespace ARC {
	class CColor : public TVec4<>
	{
	public:
		CColor() : TVec4<>(TVec4<>::ZeroVector) {}
		CColor(TVec4<> _) : TVec4<>(_) {};
		CColor(float _r, float _g, float _b, float _a) : TVec4<>(_r, _g, _b, _a) {};

		static CColor Red;
		static CColor Green;
		static CColor Blue;
	};
}