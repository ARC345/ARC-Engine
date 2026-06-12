#pragma once
#include "Event.h"
#include "..\Types\vector.h"

namespace arc {
	class ARC_API mouse_moved_event : public event
	{
	public:
		mouse_moved_event(vec2 mouse_xy) :
			mouse_pos_(mouse_xy) {}
		mouse_moved_event(float mouse_x, float mouse_y) :
			mouse_pos_(vec2(mouse_x, mouse_y)) {
			}

		inline vec2 get_pos() const {return mouse_pos_;};

		virtual string to_string() const {
			std::stringstream ss;
			ss << get_name() << "[" << get_pos().x << "," << get_pos().y << "]";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
		EVENT_CLASS_TYPE(MouseMoved)

	private:
		vec2 mouse_pos_;
	};

	class ARC_API mouse_scrolled_event : public event
	{
		public:
			mouse_scrolled_event(vec2 offset) :
				offset_(offset) {}
			mouse_scrolled_event(float offset_x, float offset_y) :
				offset_(vec2(offset_y, offset_y)) {}
		
			virtual string to_string() const override {
				std::stringstream ss;
				ss << get_name() << "[" << get_x_offset() << "," << get_y_offset() << "]";
				return ss.str();
			}

			inline vec2 get_offset() const { return offset_; }
			inline float get_x_offset() const { return offset_[0]; }
			inline float get_y_offset() const { return offset_[1]; }

			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
			EVENT_CLASS_TYPE(MouseScrolled)
		private: 
			vec2 offset_;
	};

	class ARC_API CMouseButtonEvent : public event
	{
	public:
		inline uint get_mouse_button() const { return mouse_button_; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
	protected:
		CMouseButtonEvent(uint button) : mouse_button_(button) {}
		uint mouse_button_;
	};

	class ARC_API mouse_button_pressed_event : public CMouseButtonEvent
	{
		public:
			mouse_button_pressed_event(uint button) : CMouseButtonEvent(button) {}

			virtual string to_string() const override {
				std::stringstream ss;
				ss << get_name() << " [" << mouse_button_ << "]";
				return ss.str();
			}

			EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class ARC_API mouse_button_released_event : public CMouseButtonEvent
	{
		public:
			mouse_button_released_event(uint button) : CMouseButtonEvent(button) {}

			virtual string to_string() const override {
				std::stringstream ss;
				ss << get_name() << " [" << mouse_button_ << "]";
				return ss.str();
			}

			EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}