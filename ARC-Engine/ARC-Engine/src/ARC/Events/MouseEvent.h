#pragma once
#include "Event.h"

namespace ARC {
	class ARC_API CMouseMovedEvent : public CKeyEvent
	{
	public:
		CMouseMovedEvent(int p_KeyCode, FVec2<float> p_MouseXY) :
			m_KeyCode(p_KeyCode), m_MouseXY(p_MouseXY) {}
		CMouseMovedEvent(int p_KeyCode, float p_MouseX, float p_MouseY) :
			m_KeyCode(p_KeyCode), m_MouseXY(FVec2<float>(p_MouseY, p_MouseY)) {}

		inline FVec2<float> GetXY() const {return m_MouseXY;};
		inline float GetX() const {return m_MouseXY.x();};
		inline float GetY() const {return m_MouseXY.y();};

		virtual std::string ToString() const {
			std::stringstream ss;
			ss << GetName() << " [" << m_KeyCode << "]: " << GetX() << "," << GetY();
		}

		EVENT_CLASS_CATEGORY(EEC_Mouse | EEC_Input)
		EVENT_CLASS_TYPE(MouseMoved)

	private:
		FVec2<float> m_MouseXY;
	};

	class ARC_API CMouseScrolledEvent : public CMouseButtonEvent
	{
		public:
			CMouseScrolledEvent(FVec2<float> p_Offset) :
				m_Offset(p_Offset) {}
			CMouseScrolledEvent(float p_OffsetX, float p_OffsetY) :
				m_Offset(FVec2<float>(p_OffsetY, p_OffsetY)) {}
		
			virtual std::string ToString() const override {
				std::stringstream ss;
				ss << GetName() << " [" << m_MouseButton << "]";
				return ss.str();
			}

			inline FVec2<float> GetOffset() const { return m_Offset; }
			inline float GetXOffset() const { return m_Offset.x(); }
			inline float GetYOffset() const { return m_Offset.y(); }

			EVENT_CLASS_CATEGORY(EEC_Mouse | EEC_Input)
			EVENT_CLASS_TYPE(MouseScrolled)
		private: 
			FVec2<float> m_Offset;
	};

	class ARC_API CMouseButtonEvent : public CEvent
	{
	public:
		inline uint GetMouseButton() const { return m_MouseButton; }

		EVENT_CLASS_CATEGORY(EEC_Mouse | EEC_Input)
	protected:
		CMouseButtonEvent(uint p_Button) : m_MouseButton(p_Button) {}
		uint m_MouseButton;
	};

	class ARC_API CMouseButtonPressedEvent : public CMouseButtonEvent
	{
		public:
			CMouseButtonPressedEvent(uint p_Button) : CMouseButtonEvent(p_Button) {}

			virtual std::string ToString() const override {
				std::stringstream ss;
				ss << GetName() << " [" << m_MouseButton << "]";
				return ss.str();
			}

			EVENT_CLASS_TYPE(MouseButtonPressed)
		protected:
			CMouseButtonEvent(uint p_Button) : m_MouseButton(p_Button){}
			uint m_MouseButton;
	};

	class ARC_API CMouseButtonReleasedEvent : public CMouseButtonEvent
	{
		public:
			CMouseButtonReleasedEvent(uint p_Button) : CMouseButtonEvent(p_Button) {}

			virtual std::string ToString() const override {
				std::stringstream ss;
				ss << GetName() << " [" << m_MouseButton << "]";
				return ss.str();
			}

			EVENT_CLASS_TYPE(MouseButtonReleased)
	protected:
		CMouseButtonEvent(uint p_Button) : m_MouseButton(p_Button) {}
		uint m_MouseButton;
	};
}