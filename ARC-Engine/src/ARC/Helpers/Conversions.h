#pragma once

namespace ARC
{
	// treated as a static class
	template <typename To, typename From>
	struct SConvert
	{
		static inline 
		bool CanConv(const From& p)
		{ return true; }
		
		static inline
		To Conv(const From& p) 
		{ return static_cast<To>(p); }
	};
}