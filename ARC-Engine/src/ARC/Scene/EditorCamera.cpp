#include "arc_pch.h"
#include "EditorCamera.h"

#include "ARC/Input/Input.h"
#include "ARC/Input/KeyCodes.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "../Events/MouseEvent.h"

namespace ARC {

	CEditorCamera::CEditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: mFOV(fov), mAspectRatio(aspectRatio), mNearClip(nearClip), mFarClip(farClip), CCamera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		UpdateView();
	}

	void CEditorCamera::UpdateProjection()
	{
		mAspectRatio = mViewportWidth / mViewportHeight;
		mProjection = glm::perspective(glm::radians(mFOV), mAspectRatio, mNearClip, mFarClip);
	}

	void CEditorCamera::UpdateView()
	{
		// mYaw = mPitch = 0.0f; // Lock the camera's rotation
		mPosition = CalculatePosition();

		glm::quat orientation = GetOrientation();
		mViewMatrix = glm::translate(glm::mat4(1.0f), mPosition) * glm::toMat4(orientation);
		mViewMatrix = glm::inverse(mViewMatrix);
	}

	std::pair<float, float> CEditorCamera::PanSpeed() const
	{
		float x = std::min(mViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(mViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float CEditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float CEditorCamera::ZoomSpeed() const
	{
		float distance = mDistance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void CEditorCamera::OnUpdate(float ts)
	{
		if (CInput::IsKeyPressed(ARC_KEY_LEFT_ALT))
		{
			const FGLMVec2& mouse =  (const FGLMVec2&)CInput::GetMouseXY();
			FGLMVec2 delta = (mouse - mInitialMousePosition) * 0.003f;
			mInitialMousePosition = mouse;

			if (CInput::IsMouseButtonPressed(ARC_MOUSE_BUTTON_MIDDLE))
				MousePan(delta);
			else if (CInput::IsMouseButtonPressed(ARC_MOUSE_BUTTON_LEFT))
				MouseRotate(delta);
			else if (CInput::IsMouseButtonPressed(ARC_MOUSE_BUTTON_RIGHT))
				MouseZoom(delta.y);
		}

		UpdateView();
	}

	void CEditorCamera::OnEvent(CEvent& e)
	{
		CEventDispatcher dispatcher(e);
		dispatcher.Dispatch<CMouseScrolledEvent>(BIND_FN(&CEditorCamera::OnMouseScroll));
	}

	bool CEditorCamera::OnMouseScroll(CMouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	void CEditorCamera::MousePan(const FGLMVec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		mFocalPoint += -GetRightDirection() * delta.x * xSpeed * mDistance;
		mFocalPoint += GetUpDirection() * delta.y * ySpeed * mDistance;
	}

	void CEditorCamera::MouseRotate(const FGLMVec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		mYaw += yawSign * delta.x * RotationSpeed();
		mPitch += delta.y * RotationSpeed();
	}

	void CEditorCamera::MouseZoom(float delta)
	{
		mDistance -= delta * ZoomSpeed();
		if (mDistance < 1.0f)
		{
			mFocalPoint += GetForwardDirection();
			mDistance = 1.0f;
		}
	}

	FGLMVec3 CEditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), FGLMVec3(0.0f, 1.0f, 0.0f));
	}

	FGLMVec3 CEditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), FGLMVec3(1.0f, 0.0f, 0.0f));
	}

	FGLMVec3 CEditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), FGLMVec3(0.0f, 0.0f, -1.0f));
	}

	FGLMVec3 CEditorCamera::CalculatePosition() const
	{
		return mFocalPoint - GetForwardDirection() * mDistance;
	}

	glm::quat CEditorCamera::GetOrientation() const
	{
		return glm::quat(FGLMVec3(-mPitch, -mYaw, 0.0f));
	}
}