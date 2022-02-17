#pragma once
#include "Event.h"
#include "..\Types\vector.h"

namespace ARC {
	class ARC_API CMouseMovedEvent : public CEvent
	{
	public:
		CMouseMovedEvent(FVec2 p_MouseXY) :
			m_MouseXY(p_MouseXY) {}
		CMouseMovedEvent(float p_MouseX, float p_MouseY) :
			m_MouseXY(FVec2(p_MouseX, p_MouseY)) {
			}

		inline FVec2 GetXY() const {return m_MouseXY;};
		inline float GetX() const {return m_MouseXY[0];};
		inline float GetY() const {return m_MouseXY[1];};

		virtual std::string ToString() const {
			std::stringstream ss;
			ss << GetName() << "[" << GetX() << "," << GetY() << "]";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(Mouse | Input)
		EVENT_CLASS_TYPE(MouseMoved)

	private:
		FVec2 m_MouseXY;
	};

	class ARC_API CMouseScrolledEvent : public CEvent
	{
		public:
			CMouseScrolledEvent(FVec2 p_Offset) :
				m_Offset(p_Offset) {}
			CMouseScrolledEvent(float p_OffsetX, float p_OffsetY) :
				m_Offset(FVec2(p_OffsetY, p_OffsetY)) {}
		
			virtual std::string ToString() const override {
				std::stringstream ss;
				ss << GetName() << "[" << GetXOffset() << "," << GetYOffset() << "]";
				return ss.str();
			}

			inline FVec2 GetOffset() const { return m_Offset; }
			inline float GetXOffset() const { return m_Offset[0]; }
			inline float GetYOffset() const { return m_Offset[1]; }

			EVENT_CLASS_CATEGORY(Mouse | Input)
			EVENT_CLASS_TYPE(MouseScrolled)
		private: 
			FVec2 m_Offset;
	};

	class ARC_API CMouseButtonEvent : public CEvent
	{
	public:
		inline uint GetMouseButton() const { return m_MouseButton; }

		EVENT_CLASS_CATEGORY(Mouse | Input)
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
	};
}