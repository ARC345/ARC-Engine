#pragma once
#include "Event.h"
#include "Types/vector.h"

namespace ARC {
	class ARC_API CWindowResizeEvent : public CEvent
	{
	public:
		CWindowResizeEvent(FVec2<float>& p_Dimentions) :
			m_Dimentions(p_Dimentions) {}
		CWindowResizeEvent(float p_DimentionsX, float p_DimentionsY) :
			m_Dimentions(FVec2<float>(p_DimentionsX, p_DimentionsY)) {}

		inline FVec2<float> GetXY() const { return m_Dimentions; };
		inline float GetX() const { return GetXY().x(); };
		inline float GetY() const { return GetXY().y(); };

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << GetName() << ": " << GetX() << ", " << GetY();
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EEC_Application)
		EVENT_CLASS_TYPE(WindowResize)

	private:
		FVec2<float> m_Dimentions;
	};
	
	class ARC_API CWindowCloseEvent : public CEvent
	{
	public:
		CWindowCloseEvent(){}

		EVENT_CLASS_CATEGORY(EEC_Application)
		EVENT_CLASS_TYPE(WindowClose)
	};
	
	class ARC_API CAppTick : public CEvent
	{
	public:
		CAppTick(){}

		EVENT_CLASS_CATEGORY(EEC_Application)
		EVENT_CLASS_TYPE(AppTick)
	};
	
	class ARC_API CAppUpdate : public CEvent
	{
	public:
		CAppUpdate(){}

		EVENT_CLASS_CATEGORY(EEC_Application)
		EVENT_CLASS_TYPE(AppUpdate)
	};
	
	class ARC_API CAppRender : public CEvent
	{
	public:
		CAppRender(){}

		EVENT_CLASS_CATEGORY(EEC_Application)
		EVENT_CLASS_TYPE(AppRender)
	};
}