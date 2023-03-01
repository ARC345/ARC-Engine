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
		mCurrentProjectionType = EProjectionType::Orthographic;
		mOrthographicSize = pSize;
		mOrthographicNear = pNearClip;
		mOrthographicFar = pFarClip;
		RecalculateProjection();
	}

	void CSceneCamera::SetPerspective(float pFOV, float pNearClip, float pFarClip)
	{
		mCurrentProjectionType = EProjectionType::Perspective;
		mPerspectiveFOV = pFOV;
		mPerspectiveNear = pNearClip;
		mPerspectiveFar = pFarClip;
		RecalculateProjection();
	}

	void CSceneCamera::SetViewportSize(TVec2<uint32_t> pSize)
	{
		mAspectRatio = (float)pSize.x / (float)pSize.y;
		RecalculateProjection();
	}

	void CSceneCamera::RecalculateProjection()
	{
		switch (mCurrentProjectionType)
		{
		case EProjectionType::Orthographic:
		{
			float orthoLeft = -mOrthographicSize * mAspectRatio * 0.5f;
			float orthoRight = mOrthographicSize * mAspectRatio * 0.5f;
			float orthoBottom = -mOrthographicSize * 0.5f;
			float orthoTop = mOrthographicSize * 0.5f;
			mProjection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, mOrthographicNear, mOrthographicFar);
			break;
		}
		case EProjectionType::Perspective:
		{
			mProjection = glm::perspective(mPerspectiveFOV, mAspectRatio, mPerspectiveNear, mPerspectiveFar);
			break;
		}
		default:
			break;
		}
	}
}