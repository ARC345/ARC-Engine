#include "arc_pch.h"
#include "EditorCamera.h"

#include "ARC/Input/Input.h"
#include "ARC/Input/KeyCodes.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "../Events/MouseEvent.h"

namespace arc {

	editor_camera::editor_camera(float fov, float aspectRatio, float nearClip, float farClip)
		: fov_(fov), aspect_ratio_(aspectRatio), near_clip_(nearClip), far_clip_(farClip), camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		UpdateView();
	}

	void editor_camera::UpdateProjection()
	{
		aspect_ratio_ = viewport_width_ / viewport_height_;
		projection_ = glm::perspective(glm::radians(fov_), aspect_ratio_, near_clip_, far_clip_);
	}

	void editor_camera::UpdateView()
	{
		// yaw_ = pitch_ = 0.0f; // Lock the camera's rotation
		position_ = CalculatePosition();

		glm::quat orientation = get_orientation();
		view_matrix_ = glm::translate(glm::mat4(1.0f), position_) * glm::toMat4(orientation);
		view_matrix_ = glm::inverse(view_matrix_);
	}

	std::pair<float, float> editor_camera::PanSpeed() const
	{
		float x = std::min(viewport_width_ / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(viewport_height_ / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float editor_camera::RotationSpeed() const
	{
		return 0.8f;
	}

	float editor_camera::ZoomSpeed() const
	{
		float distance = distance_ * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void editor_camera::on_update(float ts)
	{
		if (input::is_key_pressed(key::LeftAlt))
		{
			const vec2& mouse =  (const vec2&)input::get_mouse_xy();
			vec2 delta = (mouse - initial_mouse_position_) * 0.003f;
			initial_mouse_position_ = mouse;

			if (input::is_mouse_button_pressed(mouse::ButtonMiddle))
				MousePan(delta);
			else if (input::is_mouse_button_pressed(mouse::ButtonLeft))
				MouseRotate(delta);
			else if (input::is_mouse_button_pressed(mouse::ButtonRight))
				MouseZoom(delta.y);
		}

		UpdateView();
	}

	void editor_camera::on_event(event& e)
	{
		event_dispatcher dispatcher(e);
		dispatcher.Dispatch<mouse_scrolled_event>(BIND_FN(&editor_camera::OnMouseScroll));
	}

	bool editor_camera::OnMouseScroll(mouse_scrolled_event& e)
	{
		float delta = e.get_y_offset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	void editor_camera::MousePan(const vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		focal_point_ += -get_right_direction() * delta.x * xSpeed * distance_;
		focal_point_ += get_up_direction() * delta.y * ySpeed * distance_;
	}

	void editor_camera::MouseRotate(const vec2& delta)
	{
		float yawSign = get_up_direction().y < 0 ? -1.0f : 1.0f;
		yaw_ += yawSign * delta.x * RotationSpeed();
		pitch_ += delta.y * RotationSpeed();
	}

	void editor_camera::MouseZoom(float delta)
	{
		distance_ -= delta * ZoomSpeed();
		if (distance_ < 1.0f)
		{
			focal_point_ += get_forward_direction();
			distance_ = 1.0f;
		}
	}

	vec3 editor_camera::get_up_direction() const
	{
		return glm::rotate(get_orientation(), vec3(0.0f, 1.0f, 0.0f));
	}

	vec3 editor_camera::get_right_direction() const
	{
		return glm::rotate(get_orientation(), vec3(1.0f, 0.0f, 0.0f));
	}

	vec3 editor_camera::get_forward_direction() const
	{
		return glm::rotate(get_orientation(), vec3(0.0f, 0.0f, -1.0f));
	}

	vec3 editor_camera::CalculatePosition() const
	{
		return focal_point_ - get_forward_direction() * distance_;
	}

	glm::quat editor_camera::get_orientation() const
	{
		return glm::quat(vec3(-pitch_, -yaw_, 0.0f));
	}
}