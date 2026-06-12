#pragma once

#include "ARC/Core/Core.h"

#include "ARC/Helpers/Enum.h"

#include "PCH/arc_pch.h"
#include "ARC/Helpers/Helpers.h"
#include <spdlog/fmt/ostr.h>
#include "ARC/Core/Macros.h"

namespace arc {
	enum class event_type
	{
		None = 0,
		WindowClose,
		WindowResize,
		WindowFocus,
		WindowLostFocus,
		WindowMoved,

		AppTick,
		AppUpdate,
		AppRender,

		KeyPressed,
		KeyReleased,
		KeyTyped,
		
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
	};
	enum event_category
	{
		None = 0,
		EventCategoryApplication = sbit(0),
		EventCategoryInput		= sbit(1),
		EventCategoryKeyboard	= sbit(2),
		EventCategoryMouse		= sbit(3),
		EventCategoryMouseButton	= sbit(4)
	};
	#define EVENT_CLASS_TYPE(type) \
		static event_type get_static_type() {return COMBINE2(event_type::, type);}\
		virtual event_type get_event_type() const override {return get_static_type();}\
		virtual const char* get_name() const override {return #type;}

	#define EVENT_CLASS_CATEGORY(category) virtual uint get_category_flags() const override { return category; }

	class ARC_API event
	{
		friend class event_dispatcher;
	public:
		event() : bHandled(0) {}
		virtual ~event() = default;

		virtual event_type get_event_type() const = 0;
		virtual uint get_category_flags() const = 0;

		// get event name
		virtual const char* get_name() const = 0;
		
		// converts event to string
		virtual string to_string() const { return string(get_name()); };

		inline bool is_in_category(event_category category) {
			return get_category_flags() & category;
		}

		uint bHandled : 1;
	};

	class event_dispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
		

	public:
		event_dispatcher(event& event) : event_(event) {}

		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (event_.get_event_type() == T::get_static_type())
			{
				event_.bHandled |= func(static_cast<T&>(event_));
				return true;
			}
			return false;
		}
	private:
		event& event_;
	};

	inline std::ostream& operator<<(std::ostream& os, const event& e) {
		return os << e.to_string();
	}
}