#pragma once

namespace arc
{
	// treated as a static class
	template <typename To, typename From>
	struct convert
	{
		static inline 
		bool CanConv(const From& p)
		{ return true; }
		
		static inline
		To Conv(const From& p) 
		{ return static_cast<To>(p); }
	};
}