#pragma once
#include "Event.h"
#include "..\Types\vector.h"

namespace ARC {
	class ARC_API CMouseMovedEvent : public CEvent
	{
	public:
		CMouseMovedEvent(FVec2 p_MouseXY) :
			mMousePos(p_MouseXY) {}
		CMouseMovedEvent(float p_MouseX, float p_MouseY) :
			mMousePos(FVec2(p_MouseX, p_MouseY)) {
			}

		inline FVec2 GetPos() const {return mMousePos;};

		virtual TString ToString() const {
			std::stringstream ss;
			ss << GetName() << "[" << GetPos().x << "," << GetPos().y << "]";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
		EVENT_CLASS_TYPE(MouseMoved)

	private:
		FVec2 mMousePos;
	};

	class ARC_API CMouseScrolledEvent : public CEvent
	{
		public:
			CMouseScrolledEvent(FVec2 p_Offset) :
				mOffset(p_Offset) {}
			CMouseScrolledEvent(float p_OffsetX, float p_OffsetY) :
				mOffset(FVec2(p_OffsetY, p_OffsetY)) {}
		
			virtual TString ToString() const override {
				std::stringstream ss;
				ss << GetName() << "[" << GetXOffset() << "," << GetYOffset() << "]";
				return ss.str();
			}

			inline FVec2 GetOffset() const { return mOffset; }
			inline float GetXOffset() const { return mOffset[0]; }
			inline float GetYOffset() const { return mOffset[1]; }

			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
			EVENT_CLASS_TYPE(MouseScrolled)
		private: 
			FVec2 mOffset;
	};

	class ARC_API CMouseButtonEvent : public CEvent
	{
	public:
		inline TUInt GetMouseButton() const { return mMouseButton; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		CMouseButtonEvent(TUInt p_Button) : mMouseButton(p_Button) {}
		TUInt mMouseButton;
	};

	class ARC_API CMouseButtonPressedEvent : public CMouseButtonEvent
	{
		public:
			CMouseButtonPressedEvent(TUInt p_Button) : CMouseButtonEvent(p_Button) {}

			virtual TString ToString() const override {
				std::stringstream ss;
				ss << GetName() << " [" << mMouseButton << "]";
				return ss.str();
			}

			EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class ARC_API CMouseButtonReleasedEvent : public CMouseButtonEvent
	{
		public:
			CMouseButtonReleasedEvent(TUInt p_Button) : CMouseButtonEvent(p_Button) {}

			virtual TString ToString() const override {
				std::stringstream ss;
				ss << GetName() << " [" << mMouseButton << "]";
				return ss.str();
			}

			EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}