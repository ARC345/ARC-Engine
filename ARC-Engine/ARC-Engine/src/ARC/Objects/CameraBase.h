#pragma once

#include"..\..\..\out\glm\glm\glm.hpp"

namespace ARC {
	class CPrimitive2D
	{
	public:
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		float Rotation = 0.0f;
	};

	class COrthographicCameraBase : public CPrimitive2D
	{
	public:
		COrthographicCameraBase(float _Left, float _Right, float _Bottom, float _Top);
		
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; };
		void RecalculateViewProjectionMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
	};
}