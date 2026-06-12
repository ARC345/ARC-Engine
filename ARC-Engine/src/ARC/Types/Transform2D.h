#pragma once
#include "Vector.h"

namespace arc {
	template<typename T>
	class transform_2d : public type_base
	{
		ARC_TYPE();

	public:
		transform_2d() : Location(vec3<T>::ZeroVector()), Rotation(0), Scale(vec2<T>::OneVector()) {}
		transform_2d(vec3<T> l, T r, vec2<T> s) : Location(l), Rotation(r), Scale(s) {}
		
		vec3<T> Location;
		T Rotation;
		vec2<T> Scale;

		static const transform_2d<T>& default_transform() { static transform_2d<T> s;  return s; };
	};

	using transform_2d = transform_2d<float>;
}