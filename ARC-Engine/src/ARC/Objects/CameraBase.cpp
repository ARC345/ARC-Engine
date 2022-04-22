#include "arc_pch.h"
#include "CameraBase.h"

#include <glm/gtc/matrix_transform.hpp>

namespace ARC {
	COrthographicCameraBase::COrthographicCameraBase(float _Left, float _Right, float _Bottom, float _Top) :
		m_ProjectionMatrix(glm::ortho(_Left, _Right, _Bottom, _Top, -1.0f, 1.0f)),
		m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * ViewMatrix();
	}

	void COrthographicCameraBase::SetProjection(float _Left, float _Right, float _Bottom, float _Top)
	{
		m_ProjectionMatrix = glm::ortho(_Left, _Right, _Bottom, _Top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * ViewMatrix();
	}

	void COrthographicCameraBase::RecalculateViewProjectionMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * ViewMatrix();
	}		
}