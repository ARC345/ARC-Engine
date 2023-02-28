#pragma once
#include "ARC\Renderer\Camera.h"

namespace ARC
{
	struct CSceneCamera : public CCamera
	{
	public:
		enum class EProjectionType { Perspective, Orthographic };

		CSceneCamera();
		virtual ~CSceneCamera() = default;

		EProjectionType GetProjectionType() const { return mCurrentProjectionType; }
		void SetViewportSize(TVec2<uint32_t> p_NewSize);

		void SetOrthographic(float pSize, float pNearClip, float pFarClip);
		void SetPerspective(float pFOV, float pNearClip, float pFarClip);

		float GetOrthographicSize() const { return mOrthographicSize; }
		float GetOrthographicNearClip() const { return mOrthographicNear; }
		float GetOrthographicFarClip() const { return mOrthographicFar; }
		
		float GetPerspectiveFOV() const { return mPerspectiveFOV; }
		float GetPerspectiveNearClip() const { return mPerspectiveNear; }
		float GetPerspectiveFarClip() const { return mPerspectiveFar; }
		
		void SetProjectionType(EProjectionType pProjectionType) { mCurrentProjectionType = pProjectionType; RecalculateProjection(); }

		void SetOrthographicSize(float _) { mOrthographicSize = _;	RecalculateProjection(); }
		void SetOrthographicNearClip(float _) { mOrthographicNear = _;	RecalculateProjection(); }
		void SetOrthographicFarClip(float _) { mOrthographicFar = _; RecalculateProjection(); }
		
		void SetPerspectiveFOV(float _) { mPerspectiveFOV = _; RecalculateProjection(); }
		void SetPerspectiveNearClip(float _) { mPerspectiveNear = _; RecalculateProjection(); }
		void SetPerspectiveFarClip(float _) { mPerspectiveFar = _; RecalculateProjection(); }

	private:
		void RecalculateProjection();
	private:
		EProjectionType mCurrentProjectionType = EProjectionType::Orthographic;

		float mOrthographicSize = 16.0f;
		float mOrthographicNear = -1.0f;
		float mOrthographicFar = 1.0f;

		float mPerspectiveFOV = SMath::Conv<ERotType::Degrees, ERotType::Radians>(45.f);
		float mPerspectiveNear = 0.01f;
		float mPerspectiveFar = 1.f;

		float mAspectRatio = 0.f;
	};
}