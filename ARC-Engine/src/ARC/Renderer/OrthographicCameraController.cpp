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
namespace arc {
	orthographic_camera_controller::orthographic_camera_controller(float aspect_ratio, bool _bCanRotate) :
		aspect_ratio_(aspect_ratio),
		camera_(-aspect_ratio_ *  zoom_level_, aspect_ratio_ *  zoom_level_, -zoom_level_, zoom_level_),
		can_rotate_(_bCanRotate)
	{
	}

	void orthographic_camera_controller::on_update(float delta_time)
	{
		ARC_PROFILE_FUNCTION();

		if (input::is_key_pressed(key::A))
		{
			camera_.Position.x -= cos(glm::radians(camera_.Rotation)) * cam_move_speed_ * delta_time;
			camera_.Position.y -= sin(glm::radians(camera_.Rotation)) * cam_move_speed_ * delta_time;
			needs_recalculation_ = 1u;
		}
		else if (input::is_key_pressed(key::D))
		{
			camera_.Position.x += cos(glm::radians(camera_.Rotation)) * cam_move_speed_ * delta_time;
			camera_.Position.y += sin(glm::radians(camera_.Rotation)) * cam_move_speed_ * delta_time;
			needs_recalculation_ = 1u;
		}

		if (input::is_key_pressed(key::W))
		{
			camera_.Position.x += -sin(glm::radians(camera_.Rotation)) * cam_move_speed_ * delta_time;
			camera_.Position.y += cos(glm::radians(camera_.Rotation)) * cam_move_speed_ * delta_time;
			needs_recalculation_ = 1u;
		}
		else if (input::is_key_pressed(key::S))
		{
			camera_.Position.x -= -sin(glm::radians(camera_.Rotation)) * cam_move_speed_  * delta_time;
			camera_.Position.y -= cos(glm::radians(camera_.Rotation)) * cam_move_speed_ * delta_time;
			needs_recalculation_ = 1u;
		}

		if (input::is_key_pressed(key::A))
		{
			camera_.Position.x -= cam_move_speed_ * delta_time;
			needs_recalculation_ = 1u;
		}
		if (input::is_key_pressed(key::D))
		{
			camera_.Position.x += cam_move_speed_ * delta_time;
			needs_recalculation_ = 1u;
		}
		if (input::is_key_pressed(key::W))
		{
			camera_.Position.y += cam_move_speed_ * delta_time;
			needs_recalculation_ = 1u;
		}
		if (input::is_key_pressed(key::S))
		{
			camera_.Position.y -= cam_move_speed_ * delta_time;
			needs_recalculation_ = 1u;
		}

		if(can_rotate_) {
			if (input::is_key_pressed(key::Q))
			{
				camera_.Rotation += cam_rot_speed_ * delta_time;
				needs_recalculation_ = 1u;
			}
			if (input::is_key_pressed(key::E))
			{
				camera_.Rotation -= cam_rot_speed_ * delta_time;
				needs_recalculation_ = 1u;
			}
		}

		cam_move_speed_ = zoom_level_;

		if (needs_recalculation_)
		{
			camera_.RecalculateViewProjectionMatrix();
		}

	}

	void orthographic_camera_controller::on_event(event& event)
	{
		ARC_PROFILE_FUNCTION();
		event_dispatcher dispatcher(event);
		dispatcher.Dispatch<mouse_scrolled_event>(BIND_FN(&orthographic_camera_controller::OnMouseScrolledEvent));
		dispatcher.Dispatch<window_resize_event>(BIND_FN(&orthographic_camera_controller::OnWindowResized));
	}

	void orthographic_camera_controller::OnResize(float width, float height)
	{
		aspect_ratio_ = width / height;
		CalculateView();
	}

	void orthographic_camera_controller::CalculateView()
	{
		camera_.set_projection(-aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_, -zoom_level_, zoom_level_);
	}

	bool orthographic_camera_controller::OnMouseScrolledEvent(mouse_scrolled_event& event)
	{
		ARC_PROFILE_FUNCTION();
		zoom_level_ -= event.get_y_offset() * 0.25f;
		zoom_level_ = std::max(zoom_level_, 0.25f);
		CalculateView();
		return false;
	}

	bool orthographic_camera_controller::OnWindowResized(window_resize_event& event)
	{
		ARC_PROFILE_FUNCTION();
		OnResize((float)event.get_x(), (float)event.get_y());
		return false;
	}
}