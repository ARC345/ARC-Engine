#pragma once
#include "Event.h"

namespace arc {

	// abstract
	class ARC_API CKeyEvent : public event
	{
		public:
			inline int get_key_code() const { return key_code_; }

			EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
		protected:
			CKeyEvent() : key_code_(0) {}
			int key_code_ = 0;
	};
	class ARC_API key_pressed_event : public CKeyEvent
	{
		public:
			key_pressed_event(int key_code, uint repeat_count) : 
				repeat_count_() {
					key_code_ = key_code;
				}
		
			virtual string to_string() const override {
				std::stringstream ss;
				ss << get_name() << " [" << key_code_ << "]: Repeated Count= " << repeat_count_;
				return ss.str();
			}

			auto get_repeat_count() const { return repeat_count_; }

			EVENT_CLASS_TYPE(KeyPressed);
		private:	
			uint repeat_count_ = 0;
	};
	class ARC_API key_released_event : public CKeyEvent
	{
		public:
			key_released_event(int key_code)
			{
				key_code_ = key_code;
			}

			virtual string to_string() const override {
				std::stringstream ss;
				ss << get_name() << " [" << key_code_ << "]";
				return ss.str();
			}
			EVENT_CLASS_TYPE(KeyReleased);
	};
	class ARC_API key_typed_event : public CKeyEvent
	{
		public:
			key_typed_event(int key_code)
			{
				key_code_ = key_code;
			}

			virtual string to_string() const override {
				std::stringstream ss;
				ss << get_name() << " [" << key_code_ << "]";
				return ss.str();
			}
			EVENT_CLASS_TYPE(KeyTyped);
	};
}