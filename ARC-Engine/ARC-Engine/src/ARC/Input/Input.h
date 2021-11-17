#pragma once
#include "ARC/Core.h"
#include "../Types/vector.h"


namespace ARC {
	class ARC_API CInput {
	public:
		inline static bool IsKeyPressed(int _keycode) { return s_Instance->IsKeyPressedImpl(_keycode); }
		inline static bool IsMouseButtonPressed(int _button) { return s_Instance->IsMouseButtonPressedImpl(_button); }
		inline static int GetKey(int _keycode) { return s_Instance->GetKeyImpl(_keycode); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
		inline static FVec2<> GetMouseXY() { return s_Instance->GetMouseXYImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(int _keycode) const = 0;
		virtual bool IsMouseButtonPressedImpl(int _button) const = 0;
		virtual int GetKeyImpl(int _keycode) = 0;
		virtual float GetMouseXImpl()  const = 0;
		virtual float GetMouseYImpl()  const = 0;
		virtual FVec2<> GetMouseXYImpl()  const = 0;
	private:
		static CInput* s_Instance;
	};
}