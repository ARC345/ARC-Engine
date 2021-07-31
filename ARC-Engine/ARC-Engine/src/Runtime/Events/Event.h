#pragma once

#include "ARC/Core.h"

#include "Helpers/Enum.h"
#include "Helpers/Macros.h"

#include <string>
#include <functional>

namespace ARC {
	enum EEventType : uint8
	{
		EET_None = 0,
		EET_WindowClose,
		EET_WindowResize,
		EET_WindowFocus,
		EET_WindowLostFocus,
		EET_WindowMoved,

		EET_AppTick,
		EET_AppUpdate,
		EET_AppRender,

		EET_KeyPressed,
		EET_KeyReleased,
		
		EET_MouseButtonPressed,
		EET_MouseButtonReleased,
		EET_MouseMoved,
		EET_MouseReleased
	};
	enum EEventCategory : uint8
	{
		EEC_None=0,
		EEC_Application = BIT(0),
		EEC_Input		= BIT(1),
		EEC_Keyboard	= BIT(2),
		EEC_Mouse		= BIT(3),
		EEC_MouseButton	= BIT(4)
	};
	#define EVENT_CLASS_TYPE(type) \
		static EEventType GetStaticType() {return EEventType::COMBINE2(EET_, type);}\
		virtual EEventType Type() const override {return GetStaticType();}\
		virtual const char* GetName() {return #type;}

	#define EVENT_CLASS_CATEGORY
}