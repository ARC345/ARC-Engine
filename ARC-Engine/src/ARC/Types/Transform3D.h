#pragma once
#include "Vector.h"
#include "Transform2D.h"

namespace ARC {
	template<typename T>
	class TTransform3D : public TTypeBase
	{
		ARC_TYPE();

	public:
		TTransform3D() : Location(TVec3<T>::ZeroVector()), Rotation(TVec3<T>::ZeroVector()), Scale(TVec3<T>::OneVector()) {}
		TTransform3D(const TVec3<T>& _L, const TVec3<T>& _R, const TVec3<T>& _S) : Location(_L), Rotation(_R), Scale(_S) {}
		
		TVec3<T> Location;
		TVec3<T> Rotation;
		TVec3<T> Scale;
	};
	
	using FTransform3D = TTransform3D<float>;

// 	template <>
// 	struct SConvert<FTransform3D, FTransform2D>
// 	{
// 		static 
// 		FTransform3D Conv(const FTransform2D& p) 
// 		{ return FTransform3D(p.Location, pScale) }
// 	};
// 	template <>
// 	struct SConvert<FTransform2D, FTransform3D>
// 	{
// 		static FTransform2D Conv(const FTransform3D& p);
// 	};
}