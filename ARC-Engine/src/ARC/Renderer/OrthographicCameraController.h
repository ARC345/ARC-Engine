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

		COrthographicCamera& GetCamera() { return mCamera; }
		const COrthographicCamera& GetCamera() const { return mCamera; }
		inline float GetZoomLevel() const { return mZoomLevel; }
		inline void SetZoomLevel(float _val) { 
			mZoomLevel = _val;
			CalculateView();
		}
	private:
		void CalculateView();

		bool OnMouseScrolledEvent(CMouseScrolledEvent& _Event);
		bool OnWindowResized(CWindowResizeEvent& _Event);
	private:
		uint8_t mbCanRotate : 1;
		uint8_t mbNeedsRecalculation : 1;

		float mCamMoveSpeed = 10.f;
		float mCamRotSpeed = 180.f;

		float mAspectRatio;
		float mZoomLevel = 1;
		COrthographicCamera mCamera;
	};
}
