#pragma once
#include "ARC/Renderer/OrthographicCamera.h"
#include "ARC/Objects/Object.h"

namespace ARC { class CMouseScrolledEvent; }
namespace ARC { class CWindowResizeEvent; }
namespace ARC { class CEvent; }

namespace ARC {
	class COrthographicCameraController : public CObject
	{
	public:
		COrthographicCameraController(float _AspectRatio, bool _bCanRotate);

		void OnUpdate(float _DeltaTime);
		void OnEvent(CEvent& _Event);
		void OnResize(float width, float height);

		COrthographicCamera& GetCamera() { return m_Camera; }
		const COrthographicCamera& GetCamera() const { return m_Camera; }
		inline float GetZoomLevel() const { return m_ZoomLevel; }
		inline void SetZoomLevel(float _val) { 
			m_ZoomLevel = _val;
			CalculateView();
		}
	private:
		void CalculateView();

		bool OnMouseScrolledEvent(CMouseScrolledEvent& _Event);
		bool OnWindowResized(CWindowResizeEvent& _Event);
	private:
		uint8_t m_bCanRotate : 1;
		uint8_t m_bNeedsRecalculation : 1;

		float m_CamMoveSpeed = 10.f;
		float m_CamRotSpeed = 180.f;

		float m_AspectRatio;
		float m_ZoomLevel = 1;
		COrthographicCamera m_Camera;
	};
}
