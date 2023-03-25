#pragma once
#include "Component.h"

namespace ARC {
	struct CCircleCollider
	{
		FVec3 Location=FVec3::ZeroVector(); // relative // conv to FVec2 //
		float Radius=1.f;
		float Restitution=1.f;
	};
	struct CPolyCollider
	{
		std::vector<FVec3> Vertices; // inOrder // conv to FVec2 //
		float Restitution = 1.f;
	};
}