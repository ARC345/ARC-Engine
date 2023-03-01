#pragma once
#include "ARC/Core/Core.h"
#include "ARC/Types/vector.h"


namespace ARC {
	class ARC_API SInput {
	public:
		static bool IsKeyPressed(int _keycode);
		static bool IsMouseButtonPressed(int _button);
		static int GetKey(int _keycode);
		static FVec2 GetMouseXY();
	};
}