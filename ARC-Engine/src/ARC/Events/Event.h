#pragma once

#include "ARC/Core/Core.h"

#include "ARC/Helpers/Enum.h"

#include "PCH/arc_pch.h"
#include "ARC/Helpers/Helpers.h"
#include <spdlog/fmt/ostr.h>
#include "ARC/Core/Macros.h"

namespace ARC {
	enum class EEventType
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
	enum EEventCategory
	{
		None = 0,
		EventCategoryApplication = SBIT(0),
		EventCategoryInput		= SBIT(1),
		EventCategoryKeyboard	= SBIT(2),
		EventCategoryMouse		= SBIT(3),
		EventCategoryMouseButton	= SBIT(4)
	};
	#define EVENT_CLASS_TYPE(type) \
		static EEventType GetStaticType() {return COMBINE2(EEventType::, type);}\
		virtual EEventType GetEventType() const override {return GetStaticType();}\
		virtual const char* GetName() const override {return #type;}

	#define EVENT_CLASS_CATEGORY(category) virtual TUint GetCategoryFlags() const override { return category; }

	class ARC_API CEvent
	{
		friend class CEventDispatcher;
	public:
		CEvent() : bHandled(0) {}
		virtual ~CEvent() = default;

		virtual EEventType GetEventType() const = 0;
		virtual TUint GetCategoryFlags() const = 0;

		// get event name
		virtual const char* GetName() const = 0;
		
		// converts event to string
		virtual TString ToString() const { return TString(GetName()); };

		inline bool IsInCategory(EEventCategory category) {
			return GetCategoryFlags() & category;
		}

		TUint bHandled : 1;
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