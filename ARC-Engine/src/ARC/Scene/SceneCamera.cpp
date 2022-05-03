#include "arc_pch.h"
#include "SceneCamera.h"
#include "glm\ext\matrix_clip_space.inl"

namespace ARC {

	CSceneCamera::CSceneCamera()
	{
		RecalculateProjection();
	}

	void CSceneCamera::SetOrthographic(float pSize, float pNearClip, float pFarClip)
	{
		m_CurrentProjectionType = EProjectionType::Orthographic;
		m_OrthographicSize = pSize;
		m_OrthographicNear = pNearClip;
		m_OrthographicFar = pFarClip;
		RecalculateProjection();
	}

	void CSceneCamera::SetPerspective(float pFOV, float pNearClip, float pFarClip)
	{
		m_CurrentProjectionType = EProjectionType::Perspective;
		m_PerspectiveFOV = pFOV;
		m_PerspectiveNear = pNearClip;
		m_PerspectiveFar = pFarClip;
		RecalculateProjection();
	}

	void CSceneCamera::SetViewportSize(TVec2<uint32_t> pSize)
	{
		m_AspectRatio = (float)pSize.x / (float)pSize.y;
		RecalculateProjection();
	}

	void CSceneCamera::RecalculateProjection()
	{
		switch (m_CurrentProjectionType)
		{
		case EProjectionType::Orthographic:
		{
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;
			m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
			break;
		}
		case EProjectionType::Perspective:
		{
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
			break;
		}
		default:
			break;
		}
	}
}