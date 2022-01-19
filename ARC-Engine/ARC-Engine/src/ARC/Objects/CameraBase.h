#pragma once

#include "Primitive2D.h"
#include"..\..\..\out\glm\glm\glm.hpp"

namespace ARC {
	class COrthographicCameraBase : public CPrimitive2D
	{
	public:
		COrthographicCameraBase(float _Left, float _Right, float _Bottom, float _Top);
		void SetProjection(float _Left, float _Right, float _Bottom, float _Top);
		
		inline const glm::mat4& GetViewProjectionMatrix() const { return ViewProjectionMatrix(); };
		inline const glm::mat4& ViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& ViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		void RecalculateViewProjectionMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
	};
}