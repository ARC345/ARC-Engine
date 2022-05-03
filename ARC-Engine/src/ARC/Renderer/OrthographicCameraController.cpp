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
		m_AspectRatio(_AspectRatio),
		m_Camera(-m_AspectRatio *  m_ZoomLevel, m_AspectRatio *  m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_bCanRotate(_bCanRotate)
	{
	}

	void COrthographicCameraController::OnUpdate(float _DeltaTime)
	{
		ARC_PROFILE_FUNCTION();

		if (CInput::IsKeyPressed(ARC_KEY_A))
		{
			m_Camera.Position.x -= cos(glm::radians(m_Camera.Rotation)) * m_CamMoveSpeed * _DeltaTime;
			m_Camera.Position.y -= sin(glm::radians(m_Camera.Rotation)) * m_CamMoveSpeed * _DeltaTime;
			m_bNeedsRecalculation = 1u;
		}
		else if (CInput::IsKeyPressed(ARC_KEY_D))
		{
			m_Camera.Position.x += cos(glm::radians(m_Camera.Rotation)) * m_CamMoveSpeed * _DeltaTime;
			m_Camera.Position.y += sin(glm::radians(m_Camera.Rotation)) * m_CamMoveSpeed * _DeltaTime;
			m_bNeedsRecalculation = 1u;
		}

		if (CInput::IsKeyPressed(ARC_KEY_W))
		{
			m_Camera.Position.x += -sin(glm::radians(m_Camera.Rotation)) * m_CamMoveSpeed * _DeltaTime;
			m_Camera.Position.y += cos(glm::radians(m_Camera.Rotation)) * m_CamMoveSpeed * _DeltaTime;
			m_bNeedsRecalculation = 1u;
		}
		else if (CInput::IsKeyPressed(ARC_KEY_S))
		{
			m_Camera.Position.x -= -sin(glm::radians(m_Camera.Rotation)) * m_CamMoveSpeed  * _DeltaTime;
			m_Camera.Position.y -= cos(glm::radians(m_Camera.Rotation)) * m_CamMoveSpeed * _DeltaTime;
			m_bNeedsRecalculation = 1u;
		}

		if (ARC::CInput::IsKeyPressed(ARC_KEY_A))
		{
			m_Camera.Position.x -= m_CamMoveSpeed * _DeltaTime;
			m_bNeedsRecalculation = 1u;
		}
		if (ARC::CInput::IsKeyPressed(ARC_KEY_D))
		{
			m_Camera.Position.x += m_CamMoveSpeed * _DeltaTime;
			m_bNeedsRecalculation = 1u;
		}
		if (ARC::CInput::IsKeyPressed(ARC_KEY_W))
		{
			m_Camera.Position.y += m_CamMoveSpeed * _DeltaTime;
			m_bNeedsRecalculation = 1u;
		}
		if (ARC::CInput::IsKeyPressed(ARC_KEY_S))
		{
			m_Camera.Position.y -= m_CamMoveSpeed * _DeltaTime;
			m_bNeedsRecalculation = 1u;
		}

		if(m_bCanRotate) {
			if (ARC::CInput::IsKeyPressed(ARC_KEY_Q))
			{
				m_Camera.Rotation += m_CamRotSpeed * _DeltaTime;
				m_bNeedsRecalculation = 1u;
			}
			if (ARC::CInput::IsKeyPressed(ARC_KEY_E))
			{
				m_Camera.Rotation -= m_CamRotSpeed * _DeltaTime;
				m_bNeedsRecalculation = 1u;
			}
		}

		m_CamMoveSpeed = m_ZoomLevel;

		if (m_bNeedsRecalculation)
		{
			m_Camera.RecalculateViewProjectionMatrix();
		}

	}

	void COrthographicCameraController::OnEvent(CEvent& _Event)
	{
		ARC_PROFILE_FUNCTION();
		ARC::CEventDispatcher dispatcher(_Event);
		dispatcher.Dispatch<ARC::CMouseScrolledEvent>(BIND_FN(&COrthographicCameraController::OnMouseScrolledEvent));
		dispatcher.Dispatch<ARC::CWindowResizeEvent>(BIND_FN(&COrthographicCameraController::OnWindowResized));
	}

	void COrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		CalculateView();
	}

	void COrthographicCameraController::CalculateView()
	{
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool COrthographicCameraController::OnMouseScrolledEvent(CMouseScrolledEvent& _Event)
	{
		ARC_PROFILE_FUNCTION();
		m_ZoomLevel -= _Event.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
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