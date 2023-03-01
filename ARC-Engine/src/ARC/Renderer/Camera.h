#pragma once

#include "ARC/Wrappers/Glm.h"

namespace ARC
{
	class CCamera
	{
	public:
		CCamera() = default;
		CCamera(const FGLMMat4& pProjection) : mProjection(pProjection){};

		const FGLMMat4& GetProjection() const { return mProjection; }

	protected:
		FGLMMat4 mProjection = FGLMMat4(1.0f);
	};
}