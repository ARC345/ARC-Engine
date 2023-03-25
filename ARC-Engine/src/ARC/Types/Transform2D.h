#pragma once
#include "Vector.h"

namespace ARC {
	template<typename T>
	class TTransform2D : public TTypeBase
	{
		ARC_TYPE();

	public:
		TTransform2D() : Location(TVec3<T>::ZeroVector()), Rotation(0), Scale(TVec2<T>::OneVector()) {}
		TTransform2D(TVec3<T> _L, T _R, TVec2<T> _S) : Location(_L), Rotation(_R), Scale(_S) {}
		
		TVec3<T> Location;
		T Rotation;
		TVec2<T> Scale;

		static const TTransform2D<T>& DefaultTransform() { static TTransform2D<T> s;  return s; };
	};

	using FTransform2D = TTransform2D<float>;
}