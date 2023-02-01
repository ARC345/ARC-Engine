#pragma once
#include "Event.h"
#include "../Types/vector.h"

namespace ARC {
	class ARC_API CWindowResizeEvent : public CEvent
	{
	public:
		CWindowResizeEvent(TVec2<int>& p_Dimentions) :
			m_Dimentions(p_Dimentions) {}
		CWindowResizeEvent(int p_DimentionsX, int p_DimentionsY) :
			m_Dimentions(TVec2<int>(p_DimentionsX, p_DimentionsY)) {}

		inline TVec2<int> GetDimentions() const { return m_Dimentions; };
		inline int GetX() const { return GetDimentions().x; };
		inline int GetY() const { return GetDimentions().y; };

		virtual TString ToString() const override {
			std::stringstream ss;
			ss << GetName() << ": " << GetX() << ", " << GetY();
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowResize)

	private:
		TVec2<int> m_Dimentions;
	};
	
	class ARC_API CWindowCloseEvent : public CEvent
	{
	public:
		CWindowCloseEvent(){}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowClose)
	};
	
	class ARC_API CAppTick : public CEvent
	{
	public:
		CAppTick(){}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppTick)
	};
	
	class ARC_API CAppUpdate : public CEvent
	{
	public:
		CAppUpdate(){}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppUpdate)
	};
	
	class ARC_API CAppRender : public CEvent
	{
	public:
		CAppRender(){}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppRender)
	};
}