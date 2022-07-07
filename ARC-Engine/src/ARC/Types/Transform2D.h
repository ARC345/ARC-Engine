#pragma once
#include "TybeBase.h"
#include "Vector.h"
#include "Glm.h"
#include "ARC\Helpers\Helpers.h"

namespace ARC {
	template<typename T = float>
	class TTransform2D : public TTypeBase
	{
		ARC_TYPE();
	public:
		TTransform2D() : Location(TVec3<T>::ZeroVector), Rotation(0), Scale( TVec2<T>(1, 1)) {};

		TVec3<T> Location;
		T Rotation;
		TVec2<T> Scale;
	};

	using FTransform2D = TTransform2D<float>;
}