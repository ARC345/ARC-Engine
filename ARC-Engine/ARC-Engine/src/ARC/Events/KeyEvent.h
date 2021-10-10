#pragma once
#include "Events/Event.h"

namespace ARC {

	// abstract
	class ARC_API CKeyEvent : public CEvent
	{
		public:
			inline int GetKeyCode() const { return m_KeyCode; }

			EVENT_CLASS_CATEGORY(EEC_Keyboard | EEC_Input)
		protected:
			CKeyEvent() : m_KeyCode(0) {}
			int m_KeyCode = 0;
	};
	class ARC_API CKeyPressedEvent : public CKeyEvent
	{
		public:
			CKeyPressedEvent(int p_KeyCode, uint p_RepeatCount) : 
				m_KeyCode(p_KeyCode), 
				m_RepeatCount() {}
		
			virtual std::string ToString() const override {
				std::stringstream ss;
				ss << GetName() << " [" << m_KeyCode << "]: Repeated Count= " << m_RepeatCount;
				return ss.str();
			}
			EVENT_CLASS_TYPE(KeyPressed);
		private:	
			uint m_RepeatCount = 0;
	};
	class ARC_API CKeyReleasedEvent : public CKeyEvent
	{
		public:
			CKeyPressedEvent(int p_KeyCode) : 
				m_KeyCode(p_KeyCode){}

			virtual std::string ToString() const override {
				std::stringstream ss;
				ss << GetName() << " [" << m_KeyCode << "]";
				return ss.str();
			}
			EVENT_CLASS_TYPE(KeyReleased);
	};
}