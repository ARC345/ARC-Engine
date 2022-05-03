#pragma once
#include "ARC\Types\Glm.h"

namespace ARC {
	class COrthographicCamera
	{
	public:
		COrthographicCamera(float _Left, float _Right, float _Bottom, float _Top);
		void SetProjection(float _Left, float _Right, float _Bottom, float _Top);
		
		inline const FGMat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const FGMat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		void RecalculateViewProjectionMatrix();

	public:
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
		float Rotation = 0.0f;
	private:
		FGMat4 m_ProjectionMatrix;
		FGMat4 m_ViewMatrix;
		FGMat4 m_ViewProjectionMatrix;
	};
}