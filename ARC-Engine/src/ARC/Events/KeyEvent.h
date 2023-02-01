#pragma once
#include "Event.h"

namespace ARC {

	// abstract
	class ARC_API CKeyEvent : public CEvent
	{
		public:
			inline int GetKeyCode() const { return m_KeyCode; }

			EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
		protected:
			CKeyEvent() : m_KeyCode(0) {}
			int m_KeyCode = 0;
	};
	class ARC_API CKeyPressedEvent : public CKeyEvent
	{
		public:
			CKeyPressedEvent(int p_KeyCode, TUint p_RepeatCount) : 
				m_RepeatCount() {
					m_KeyCode = p_KeyCode;
				}
		
			virtual TString ToString() const override {
				std::stringstream ss;
				ss << GetName() << " [" << m_KeyCode << "]: Repeated Count= " << m_RepeatCount;
				return ss.str();
			}
			EVENT_CLASS_TYPE(KeyPressed);
		private:	
			TUint m_RepeatCount = 0;
	};
	class ARC_API CKeyReleasedEvent : public CKeyEvent
	{
		public:
			CKeyReleasedEvent(int p_KeyCode)
			{
				m_KeyCode = p_KeyCode;
			}

			virtual TString ToString() const override {
				std::stringstream ss;
				ss << GetName() << " [" << m_KeyCode << "]";
				return ss.str();
			}
			EVENT_CLASS_TYPE(KeyReleased);
	};
	class ARC_API CKeyTypedEvent : public CKeyEvent
	{
		public:
			CKeyTypedEvent(int p_KeyCode)
			{
				m_KeyCode = p_KeyCode;
			}

			virtual TString ToString() const override {
				std::stringstream ss;
				ss << GetName() << " [" << m_KeyCode << "]";
				return ss.str();
			}
			EVENT_CLASS_TYPE(KeyTyped);
	};
}