#pragma once
#include "ARC/Renderer/OrthographicCamera.h"
#include "ARC/Objects/Object.h"

namespace arc { class mouse_scrolled_event; }
namespace arc { class window_resize_event; }
namespace arc { class event; }

namespace arc {
	class orthographic_camera_controller : public object
	{
	public:
		orthographic_camera_controller(float aspect_ratio, bool _bCanRotate);

		void on_update(float delta_time);
		void on_event(event& event);
		void OnResize(float width, float height);

		orthographic_camera& get_camera() { return camera_; }
		const orthographic_camera& get_camera() const { return camera_; }
		inline float get_zoom_level() const { return zoom_level_; }
		inline void set_zoom_level(float _val) { 
			zoom_level_ = _val;
			CalculateView();
		}
	private:
		void CalculateView();

		bool OnMouseScrolledEvent(mouse_scrolled_event& event);
		bool OnWindowResized(window_resize_event& event);
	private:
		uint8_t can_rotate_ : 1;
		uint8_t needs_recalculation_ : 1;

		float cam_move_speed_ = 10.f;
		float cam_rot_speed_ = 180.f;

		float aspect_ratio_;
		float zoom_level_ = 1;
		orthographic_camera camera_;
	};
}
