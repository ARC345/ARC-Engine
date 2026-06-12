#pragma once
#include "Event.h"
#include "../Types/vector.h"

namespace arc {
	class ARC_API window_resize_event : public event
	{
	public:
		window_resize_event(vec2<int>& dimentions) :
			dimentions_(dimentions) {}
		window_resize_event(int dimentions_x, int dimentions_y) :
			dimentions_(vec2<int>(dimentions_x, dimentions_y)) {}

		inline vec2<int> get_dimentions() const { return dimentions_; };
		inline int get_x() const { return get_dimentions().x; };
		inline int get_y() const { return get_dimentions().y; };

		virtual string to_string() const override {
			std::stringstream ss;
			ss << get_name() << ": " << get_x() << ", " << get_y();
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowResize)

	private:
		vec2<int> dimentions_;
	};
	
	class ARC_API window_close_event : public event
	{
	public:
		window_close_event(){}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowClose)
	};
	
	class ARC_API CAppTick : public event
	{
	public:
		CAppTick(){}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppTick)
	};
	
	class ARC_API CAppUpdate : public event
	{
	public:
		CAppUpdate(){}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppUpdate)
	};
	
	class ARC_API CAppRender : public event
	{
	public:
		CAppRender(){}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppRender)
	};
}