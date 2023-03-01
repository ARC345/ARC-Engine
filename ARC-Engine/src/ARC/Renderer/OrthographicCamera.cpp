#include "arc_pch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace ARC {
	COrthographicCamera::COrthographicCamera(float _Left, float _Right, float _Bottom, float _Top) :
		mProjectionMatrix(glm::ortho(_Left, _Right, _Bottom, _Top, -1.0f, 1.0f)),
		mViewMatrix(1.0f)
	{
		mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
	}

	void COrthographicCamera::SetProjection(float _Left, float _Right, float _Bottom, float _Top)
	{
		mProjectionMatrix = glm::ortho(_Left, _Right, _Bottom, _Top, -1.0f, 1.0f);
		
		mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
	}

	void COrthographicCamera::RecalculateViewProjectionMatrix()
	{
		FGLMMat4 transform = glm::translate(FGLMMat4(1.0f), Position) *
			glm::rotate(FGLMMat4(1.0f), glm::radians(Rotation), glm::vec3(0, 0, 1));

		mViewMatrix = glm::inverse(transform);
		mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
	}		
}