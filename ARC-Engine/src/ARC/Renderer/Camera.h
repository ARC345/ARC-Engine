#pragma once

#include "ARC/Wrappers/Glm.h"

namespace ARC
{
	class CCamera
	{
	public:
		CCamera() = default;
		CCamera(const FGMat4& pProjection) : m_Projection(pProjection){};

		const FGMat4& GetProjection() const { return m_Projection; }

	protected:
		FGMat4 m_Projection = FGMat4(1.0f);
	};
}