#pragma once
#include "Event.h"
#include "..\Types\vector.h"

namespace ARC {
	class ARC_API CMouseMovedEvent : public CEvent
	{
	public:
		CMouseMovedEvent(FVec2 p_MouseXY) :
			m_MousePos(p_MouseXY) {}
		CMouseMovedEvent(float p_MouseX, float p_MouseY) :
			m_MousePos(FVec2(p_MouseX, p_MouseY)) {
			}

		inline FVec2 GetPos() const {return m_MousePos;};

		virtual TString ToString() const {
			std::stringstream ss;
			ss << GetName() << "[" << GetPos().x << "," << GetPos().y << "]";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
		EVENT_CLASS_TYPE(MouseMoved)

	private:
		FVec2 m_MousePos;
	};

	class ARC_API CMouseScrolledEvent : public CEvent
	{
		public:
			CMouseScrolledEvent(FVec2 p_Offset) :
				m_Offset(p_Offset) {}
			CMouseScrolledEvent(float p_OffsetX, float p_OffsetY) :
				m_Offset(FVec2(p_OffsetY, p_OffsetY)) {}
		
			virtual TString ToString() const override {
				std::stringstream ss;
				ss << GetName() << "[" << GetXOffset() << "," << GetYOffset() << "]";
				return ss.str();
			}

			inline FVec2 GetOffset() const { return m_Offset; }
			inline float GetXOffset() const { return m_Offset[0]; }
			inline float GetYOffset() const { return m_Offset[1]; }

			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
			EVENT_CLASS_TYPE(MouseScrolled)
		private: 
			FVec2 m_Offset;
	};

	class ARC_API CMouseButtonEvent : public CEvent
	{
	public:
		inline TUint GetMouseButton() const { return m_MouseButton; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		CMouseButtonEvent(TUint p_Button) : m_MouseButton(p_Button) {}
		TUint m_MouseButton;
	};

	class ARC_API CMouseButtonPressedEvent : public CMouseButtonEvent
	{
		public:
			CMouseButtonPressedEvent(TUint p_Button) : CMouseButtonEvent(p_Button) {}

			virtual TString ToString() const override {
				std::stringstream ss;
				ss << GetName() << " [" << m_MouseButton << "]";
				return ss.str();
			}

			EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class ARC_API CMouseButtonReleasedEvent : public CMouseButtonEvent
	{
		public:
			CMouseButtonReleasedEvent(TUint p_Button) : CMouseButtonEvent(p_Button) {}

			virtual TString ToString() const override {
				std::stringstream ss;
				ss << GetName() << " [" << m_MouseButton << "]";
				return ss.str();
			}

			EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}