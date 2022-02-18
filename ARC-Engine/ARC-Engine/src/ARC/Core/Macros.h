#pragma once

#include "PlatformDetection.h"

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

#define _ASSERT_DB(x) if(!(x)) __debugbreak();

#define ifnr(x) if(!(x)) return;
#define ifr(x) if((x)) return;
#define ifnr_callback(x, y) if(!(x)){y; return;}

#define COMBINE4(a,b,c,d) a##b##c##d
#define COMBINE3(a,b,c) a##b##c
#define COMBINE2(a,b) a##b

#define BIND_FN(x) std::bind(x, this, std::placeholders::_1)
#define ARC_PROFILE
#ifdef ARC_PROFILE
	#define ARC_PROFILE_SCOPE(name) ::ARC::CTimer COMBINE2(ARC_timer, __LINE__)(name, true, true)
	#define ARC_PROFILE_FUNCTION() ARC_PROFILE_SCOPE(__FUNCSIG__)
	#define ARC_PROFILE_BEGIN_SESSION(name, filepath) ::ARC::CInstrumentor::Get().BeginSession(name, filepath)
	#define ARC_PROFILE_END_SESSION() ::ARC::CInstrumentor::Get().EndSession()
#else	
	#define ARC_PROFILE_SCOPE(name)
	#define ARC_PROFILE_FUNCTION()
	#define ARC_PROFILE_BEGIN_SESSION(name, filepath)
	#define ARC_PROFILE_END_SESSION()
#endif