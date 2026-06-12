#pragma once
#include "ARC\Renderer\Camera.h"
#include "ARC\Wrappers\Glm.h"

namespace arc { class event; }
namespace arc { class mouse_scrolled_event; }

namespace arc {

	class editor_camera : public camera
	{
	public:
		editor_camera() = default;
		editor_camera(float fov, float aspectRatio, float nearClip, float farClip);

		void on_update(float ts);
		void on_event(event& e);

		inline float get_distance() const { return distance_; }
		inline void set_distance(float distance) { distance_ = distance; }

		inline void set_viewport_size(float width, float height) { viewport_width_ = width; viewport_height_ = height; UpdateProjection(); }

		const mat4& get_view_matrix() const { return view_matrix_; }
		mat4 get_view_projection_matrix() const { return projection_ * view_matrix_; }

		vec3 get_up_direction() const;
		vec3 get_right_direction() const;
		vec3 get_forward_direction() const;
		const vec3& get_position() const { return position_; }
		glm::quat get_orientation() const;

		float get_pitch() const { return pitch_; }
		float get_yaw() const { return yaw_; }
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(mouse_scrolled_event& e);

		void MousePan(const vec2& delta);
		void MouseRotate(const vec2& delta);
		void MouseZoom(float delta);

		vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float fov_ = 45.0f, aspect_ratio_ = 1.778f, near_clip_ = 0.1f, far_clip_ = 1000.0f;

		mat4 view_matrix_;
		vec3 position_ = { 0.0f, 0.0f, 0.0f };
		vec3 focal_point_ = { 0.0f, 0.0f, 0.0f };

		vec2 initial_mouse_position_ = { 0.0f, 0.0f };

		float distance_ = 10.0f;
		float pitch_ = 0.0f, yaw_ = 0.0f;

		float viewport_width_ = 1280, viewport_height_ = 720;
	};

}