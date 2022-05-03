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

		EProjectionType GetProjectionType() const { return m_CurrentProjectionType; }
		void SetViewportSize(TVec2<uint32_t> p_NewSize);

		void SetOrthographic(float pSize, float pNearClip, float pFarClip);
		void SetPerspective(float pFOV, float pNearClip, float pFarClip);

		float GetOrthographicSize() const { return m_OrthographicSize; }
		float GetOrthographicNearClip() const { return m_OrthographicNear; }
		float GetOrthographicFarClip() const { return m_OrthographicFar; }
		
		float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		
		void SetProjectionType(EProjectionType pProjectionType) { m_CurrentProjectionType = pProjectionType; RecalculateProjection(); }

		void SetOrthographicSize(float _) { m_OrthographicSize = _;	RecalculateProjection(); }
		void SetOrthographicNearClip(float _) { m_OrthographicNear = _;	RecalculateProjection(); }
		void SetOrthographicFarClip(float _) { m_OrthographicFar = _; RecalculateProjection(); }
		
		void SetPerspectiveFOV(float _) { m_PerspectiveFOV = _; RecalculateProjection(); }
		void SetPerspectiveNearClip(float _) { m_PerspectiveNear = _; RecalculateProjection(); }
		void SetPerspectiveFarClip(float _) { m_PerspectiveFar = _; RecalculateProjection(); }

	private:
		void RecalculateProjection();
	private:
		EProjectionType m_CurrentProjectionType = EProjectionType::Orthographic;

		float m_OrthographicSize = 16.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;

		float m_PerspectiveFOV = Math::Conv<Degrees, Radians>(45.f);
		float m_PerspectiveNear = 0.01f;
		float m_PerspectiveFar = 1.f;

		float m_AspectRatio = 0.f;
	};
}