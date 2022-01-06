#pragma once

#ifdef ARC_PLATFORM_WINDOWS
#ifdef ARC_DYNAIMC_LINK
	#ifdef ARC_BUILD_DLL
		#define ARC_API  __declspec(dllexport)
	#else
		#define ARC_API  __declspec(dllimport)
	#endif
#else
	#define ARC_API
#endif
#else
	#error ARC only supports Windows
#endif