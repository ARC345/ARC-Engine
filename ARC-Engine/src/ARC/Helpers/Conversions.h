#pragma once

namespace ARC
{
	// treated as a static class
	template <typename To, typename From>
	struct SConvert
	{
		static To Conv(const From& p) {
			return static_cast<To>(p);
		}
	};
}