#pragma once

#include "Vector.h"

namespace arc {

	namespace transparency_type {
		enum transparency_type {
			Opaque=1,
			Translucent=2,
			Transparent=3
		};
	}
	using transparency_type = u8;
	class color4 : public vec4
	{
	public:
		color4() : vec4(vec4::ZeroVector()) {}
		color4(const vec4& _) : vec4(_) {};
		color4(float _r, float _g, float _b, float _a) : vec4(_r, _g, _b, _a) {};

		[[nodiscard]] static
		const color4& White() { static color4 rval = color4(1.f); return rval; }
		[[nodiscard]] static
		const color4& Red() { static color4 rval = color4(1.f, 0.f, 0.f, 1.f); return rval; }
		[[nodiscard]] static
		const color4& Green() { static color4 rval = color4(0.f, 1.f, 0.f, 1.f); return rval; }
		[[nodiscard]] static
		const color4& Blue() { static color4 rval = color4(0.f, 0.f, 1.f, 1.f); return rval; }
		[[nodiscard]] static
		const color4& Yellow() { static color4 rval = color4(1.f, 1.f, 0.f, 1.f); return rval; }
		[[nodiscard]] static
		const color4& Black() { static color4 rval = color4(0.f, 0.f, 0.f, 1.f); return rval; }
	};
}