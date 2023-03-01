#pragma once
#include "Event.h"

namespace ARC {

	// abstract
	class ARC_API CKeyEvent : public CEvent
	{
		public:
			inline int GetKeyCode() const { return mKeyCode; }

			EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
		protected:
			CKeyEvent() : mKeyCode(0) {}
			int mKeyCode = 0;
	};
	class ARC_API CKeyPressedEvent : public CKeyEvent
	{
		public:
			CKeyPressedEvent(int p_KeyCode, TUInt p_RepeatCount) : 
				mRepeatCount() {
					mKeyCode = p_KeyCode;
				}
		
			virtual TString ToString() const override {
				std::stringstream ss;
				ss << GetName() << " [" << mKeyCode << "]: Repeated Count= " << mRepeatCount;
				return ss.str();
			}
			EVENT_CLASS_TYPE(KeyPressed);
		private:	
			TUInt mRepeatCount = 0;
	};
	class ARC_API CKeyReleasedEvent : public CKeyEvent
	{
		public:
			CKeyReleasedEvent(int p_KeyCode)
			{
				mKeyCode = p_KeyCode;
			}

			virtual TString ToString() const override {
				std::stringstream ss;
				ss << GetName() << " [" << mKeyCode << "]";
				return ss.str();
			}
			EVENT_CLASS_TYPE(KeyReleased);
	};
	class ARC_API CKeyTypedEvent : public CKeyEvent
	{
		public:
			CKeyTypedEvent(int p_KeyCode)
			{
				mKeyCode = p_KeyCode;
			}

			virtual TString ToString() const override {
				std::stringstream ss;
				ss << GetName() << " [" << mKeyCode << "]";
				return ss.str();
			}
			EVENT_CLASS_TYPE(KeyTyped);
	};
}