#include "arc_pch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace ARC {
	COrthographicCamera::COrthographicCamera(float _Left, float _Right, float _Bottom, float _Top) :
		m_ProjectionMatrix(glm::ortho(_Left, _Right, _Bottom, _Top, -1.0f, 1.0f)),
		m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * GetViewMatrix();
	}

	void COrthographicCamera::SetProjection(float _Left, float _Right, float _Bottom, float _Top)
	{
		m_ProjectionMatrix = glm::ortho(_Left, _Right, _Bottom, _Top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * GetViewMatrix();
	}

	void COrthographicCamera::RecalculateViewProjectionMatrix()
	{
		FGMat4 transform = glm::translate(FGMat4(1.0f), Position) *
			glm::rotate(FGMat4(1.0f), glm::radians(Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * GetViewMatrix();
	}		
}