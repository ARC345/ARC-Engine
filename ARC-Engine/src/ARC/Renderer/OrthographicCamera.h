#pragma once
#include "ARC/Wrappers/Glm.h"

namespace ARC {
	class COrthographicCamera
	{
	public:
		COrthographicCamera(float _Left, float _Right, float _Bottom, float _Top);
		void SetProjection(float _Left, float _Right, float _Bottom, float _Top);
		
		inline const FGLMMat4& GetViewMatrix() const { return mViewMatrix; }
		inline const FGLMMat4& GetViewProjectionMatrix() const { return mViewProjectionMatrix; }
		void RecalculateViewProjectionMatrix();

	public:
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
		float Rotation = 0.0f;
	private:
		FGLMMat4 mProjectionMatrix;
		FGLMMat4 mViewMatrix;
		FGLMMat4 mViewProjectionMatrix;
	};
}