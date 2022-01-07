#pragma once
#include "ARC/Helpers/DllHelpers_Macros.h"

#ifdef ARC_DEBUG
	#define ARC_ENABLE_ASSERTS
#endif

#ifdef ARC_ENABLE_ASSERTS
	#define ARC_ASSERT(x, ...) { if(!(x)) { ARC_ERROR("Assertion Failed {0}", __VA_ARGS__); __debugbreak(); } }
	#define ARC_CORE_ASSERT(x, ...) { if(!(x)) { ARC_CORE_ERROR("Assertion Failed {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define ARC_ASSERT(x, ...)
	#define ARC_CORE_ASSERT(x, ...)
#endif
