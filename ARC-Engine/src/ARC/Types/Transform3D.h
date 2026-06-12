#pragma once
#include "Vector.h"
#include "Transform2D.h"

namespace arc {
	template<typename T>
	class transform_3d : public type_base
	{
		ARC_TYPE();

	public:
		transform_3d() : Location(vec3<T>::ZeroVector()), Rotation(vec3<T>::ZeroVector()), Scale(vec3<T>::OneVector()) {}
		transform_3d(const vec3<T>& l, const vec3<T>& r, const vec3<T>& s) : Location(l), Rotation(r), Scale(s) {}
		
		vec3<T> Location;
		vec3<T> Rotation;
		vec3<T> Scale;
	};
	
	using transform_3d = transform_3d<float>;
}