#pragma once
#include "Vector.h"

namespace ARC {
	template<typename T = float>
	class TTransform2D
	{
	public:
		TTransform2D() : Location(TVec2<T>::ZeroVector), ZOrder(0), Rotation(0), Scale( TVec2<T>(1, 1)) {};

		TVec2<T> Location;
		T ZOrder;
		T Rotation;
		TVec2<T> Scale;
	};

	using FTransform2D = TTransform2D<float>;
}