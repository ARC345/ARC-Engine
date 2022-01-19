#pragma once

#include "ARC/Core/Core.h"

#include "ARC/Helpers/Enum.h"
#include "ARC/Helpers/Macros.h"

#include "PCH/arc_pch.h"
#include "ARC/Helpers/Helpers.h"
#include <spdlog/fmt/ostr.h>

namespace ARC {
	enum class EEventType
	{
		EET_None = 0,
		EET_WindowClose,
		EET_WindowResize,
		EET_WindowFocus,
		EET_WindowLostFocus,
		EET_WindowMoved,

		EET_AppTick,
		EET_AppUpdate,
		EET_AppRender,

		EET_KeyPressed,
		EET_KeyReleased,
		EET_KeyTyped,
		
		EET_MouseButtonPressed,
		EET_MouseButtonReleased,
		EET_MouseMoved,
		EET_MouseScrolled
	};
	enum EEventCategory
	{
		None = 0,
		Application = SBIT(0),
		Input		= SBIT(1),
		Keyboard	= SBIT(2),
		Mouse		= SBIT(3),
		MouseButton	= SBIT(4)
	};
	#define EVENT_CLASS_TYPE(type) \
		static EEventType GetStaticType() {return EEventType::COMBINE2(EET_, type);}\
		virtual EEventType GetEventType() const override {return GetStaticType();}\
		virtual const char* GetName() const override {return #type;}

	#define EVENT_CLASS_CATEGORY(category) virtual uint GetCategoryFlags() const override { return category; }

	class ARC_API CEvent
	{
		friend class CEventDispatcher;
	public:
		CEvent() : bHandled(0) {}
	
		virtual EEventType GetEventType() const = 0;
		virtual uint GetCategoryFlags() const = 0;

		// get event name
		virtual const char* GetName() const = 0;
		
		// converts event to string
		virtual std::string ToString() const { return std::string(GetName()); };

		inline bool IsInCategory(EEventCategory category) {
			return GetCategoryFlags() & category;
		}

		uint bHandled : 1;
	};

	class CEventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
		

	public:
		CEventDispatcher(CEvent& p_Event) : m_Event(p_Event) {}

		template<typename T, typename F>
		bool Dispatch(const F& _Func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.bHandled = _Func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		CEvent& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const CEvent& e) {
		return os << e.ToString();
	}
}