#pragma once
#include "ARC\Renderer\Camera.h"
#include "ARC\Wrappers\Glm.h"

namespace ARC { class CEvent; }
namespace ARC { class CMouseScrolledEvent; }

namespace ARC {

	class CEditorCamera : public CCamera
	{
	public:
		CEditorCamera() = default;
		CEditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(float ts);
		void OnEvent(CEvent& e);

		inline float GetDistance() const { return mDistance; }
		inline void SetDistance(float distance) { mDistance = distance; }

		inline void SetViewportSize(float width, float height) { mViewportWidth = width; mViewportHeight = height; UpdateProjection(); }

		const FGLMMat4& GetViewMatrix() const { return mViewMatrix; }
		FGLMMat4 GetViewProjectionMatrix() const { return mProjection * mViewMatrix; }

		FGLMVec3 GetUpDirection() const;
		FGLMVec3 GetRightDirection() const;
		FGLMVec3 GetForwardDirection() const;
		const FGLMVec3& GetPosition() const { return mPosition; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return mPitch; }
		float GetYaw() const { return mYaw; }
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(CMouseScrolledEvent& e);

		void MousePan(const FGLMVec2& delta);
		void MouseRotate(const FGLMVec2& delta);
		void MouseZoom(float delta);

		FGLMVec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float mFOV = 45.0f, mAspectRatio = 1.778f, mNearClip = 0.1f, mFarClip = 1000.0f;

		FGLMMat4 mViewMatrix;
		FGLMVec3 mPosition = { 0.0f, 0.0f, 0.0f };
		FGLMVec3 mFocalPoint = { 0.0f, 0.0f, 0.0f };

		FGLMVec2 mInitialMousePosition = { 0.0f, 0.0f };

		float mDistance = 10.0f;
		float mPitch = 0.0f, mYaw = 0.0f;

		float mViewportWidth = 1280, mViewportHeight = 720;
	};

}