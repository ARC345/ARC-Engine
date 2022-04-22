#pragma once
#include <glm/glm.hpp>

namespace ARC {
	class COrthographicCameraBase
	{
	public:
		COrthographicCameraBase(float _Left, float _Right, float _Bottom, float _Top);
		void SetProjection(float _Left, float _Right, float _Bottom, float _Top);
		
		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		void RecalculateViewProjectionMatrix();

	public:
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
		float Rotation = 0.0f;
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
	};
}