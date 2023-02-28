#include "arc_pch.h"
#include "OrthographicCameraController.h"
#include "ARC/Types/vector.h"
#include "ARC/Events/MouseEvent.h"
#include "ARC/Events/ApplicationEvent.h"
#include "ARC/Events/Event.h"
#include "ARC/Input/Input.h"
#include "ARC/Input/KeyCodes.h"
#include "ARC/Core/Macros.h"
#include "ARC/Profiling/Timer.h"
#include "glm/trigonometric.hpp"
namespace ARC {
	COrthographicCameraController::COrthographicCameraController(float _AspectRatio, bool _bCanRotate) :
		mAspectRatio(_AspectRatio),
		mCamera(-mAspectRatio *  mZoomLevel, mAspectRatio *  mZoomLevel, -mZoomLevel, mZoomLevel),
		mbCanRotate(_bCanRotate)
	{
	}

	void COrthographicCameraController::OnUpdate(float _DeltaTime)
	{
		ARC_PROFILE_FUNCTION();

		if (CInput::IsKeyPressed(ARC_KEY_A))
		{
			mCamera.Position.x -= cos(glm::radians(mCamera.Rotation)) * mCamMoveSpeed * _DeltaTime;
			mCamera.Position.y -= sin(glm::radians(mCamera.Rotation)) * mCamMoveSpeed * _DeltaTime;
			mbNeedsRecalculation = 1u;
		}
		else if (CInput::IsKeyPressed(ARC_KEY_D))
		{
			mCamera.Position.x += cos(glm::radians(mCamera.Rotation)) * mCamMoveSpeed * _DeltaTime;
			mCamera.Position.y += sin(glm::radians(mCamera.Rotation)) * mCamMoveSpeed * _DeltaTime;
			mbNeedsRecalculation = 1u;
		}

		if (CInput::IsKeyPressed(ARC_KEY_W))
		{
			mCamera.Position.x += -sin(glm::radians(mCamera.Rotation)) * mCamMoveSpeed * _DeltaTime;
			mCamera.Position.y += cos(glm::radians(mCamera.Rotation)) * mCamMoveSpeed * _DeltaTime;
			mbNeedsRecalculation = 1u;
		}
		else if (CInput::IsKeyPressed(ARC_KEY_S))
		{
			mCamera.Position.x -= -sin(glm::radians(mCamera.Rotation)) * mCamMoveSpeed  * _DeltaTime;
			mCamera.Position.y -= cos(glm::radians(mCamera.Rotation)) * mCamMoveSpeed * _DeltaTime;
			mbNeedsRecalculation = 1u;
		}

		if (CInput::IsKeyPressed(ARC_KEY_A))
		{
			mCamera.Position.x -= mCamMoveSpeed * _DeltaTime;
			mbNeedsRecalculation = 1u;
		}
		if (CInput::IsKeyPressed(ARC_KEY_D))
		{
			mCamera.Position.x += mCamMoveSpeed * _DeltaTime;
			mbNeedsRecalculation = 1u;
		}
		if (CInput::IsKeyPressed(ARC_KEY_W))
		{
			mCamera.Position.y += mCamMoveSpeed * _DeltaTime;
			mbNeedsRecalculation = 1u;
		}
		if (CInput::IsKeyPressed(ARC_KEY_S))
		{
			mCamera.Position.y -= mCamMoveSpeed * _DeltaTime;
			mbNeedsRecalculation = 1u;
		}

		if(mbCanRotate) {
			if (CInput::IsKeyPressed(ARC_KEY_Q))
			{
				mCamera.Rotation += mCamRotSpeed * _DeltaTime;
				mbNeedsRecalculation = 1u;
			}
			if (CInput::IsKeyPressed(ARC_KEY_E))
			{
				mCamera.Rotation -= mCamRotSpeed * _DeltaTime;
				mbNeedsRecalculation = 1u;
			}
		}

		mCamMoveSpeed = mZoomLevel;

		if (mbNeedsRecalculation)
		{
			mCamera.RecalculateViewProjectionMatrix();
		}

	}

	void COrthographicCameraController::OnEvent(CEvent& _Event)
	{
		ARC_PROFILE_FUNCTION();
		CEventDispatcher dispatcher(_Event);
		dispatcher.Dispatch<CMouseScrolledEvent>(BIND_FN(&COrthographicCameraController::OnMouseScrolledEvent));
		dispatcher.Dispatch<CWindowResizeEvent>(BIND_FN(&COrthographicCameraController::OnWindowResized));
	}

	void COrthographicCameraController::OnResize(float width, float height)
	{
		mAspectRatio = width / height;
		CalculateView();
	}

	void COrthographicCameraController::CalculateView()
	{
		mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
	}

	bool COrthographicCameraController::OnMouseScrolledEvent(CMouseScrolledEvent& _Event)
	{
		ARC_PROFILE_FUNCTION();
		mZoomLevel -= _Event.GetYOffset() * 0.25f;
		mZoomLevel = std::max(mZoomLevel, 0.25f);
		CalculateView();
		return false;
	}

	bool COrthographicCameraController::OnWindowResized(CWindowResizeEvent& _Event)
	{
		ARC_PROFILE_FUNCTION();
		OnResize((float)_Event.GetX(), (float)_Event.GetY());
		return false;
	}
}