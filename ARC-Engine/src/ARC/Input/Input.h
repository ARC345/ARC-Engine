#pragma once
#include "ARC/Core/Core.h"
#include "ARC/Types/vector.h"


namespace arc {
	class ARC_API input {
	public:
		static bool is_key_pressed(int _keycode);
		static bool is_mouse_button_pressed(int _button);
		static int get_key(int _keycode);
		static vec2 get_mouse_xy();
	};
}