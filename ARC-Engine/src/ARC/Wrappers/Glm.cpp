#include "arc_pch.h"
#include "glm\gtx\transform.hpp"
#include "Glm.h"

namespace ARC {
	FGMat4 SConvert<FGMat4, FTransform2D>::Conv(const FTransform2D& pRhs)
	{
		return glm::translate(FGMat4(1.0f), FGVec3(pRhs.Location.x, pRhs.Location.y, pRhs.Location.z)) *
			glm::rotate(FGMat4(1.0f), pRhs.Rotation, FGVec3(0, 0, 1)) *
			glm::scale(FGMat4(1.0f), FGVec3(pRhs.Scale.x, pRhs.Scale.y, 1.0f));
	}
}