#pragma once
#ifdef _WIN32
	#ifdef _WIN64
		#define ARC_PLATFORM_WINDOWS
	#else
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	* so we must check all of them (in this order)
	* to ensure that we're running on MAC
	* and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported"
	#elif TARGET_OS_IPHONE == 1
		#define ARC_PLATFORM_IOS
		#error "IOS is not supported"
	#elif TARGET_OS_MAC == 1
		#define ARC_PLATFORM_MACOS
		#error "MacOS is not supported"
	#else
		#error "Unknown Apple platform"
	#endif
#elif defined(__ANDROID__)
	#define ARC_PLATFORM_ANDROID
	#error "Android is not supported"
#elif defined(__linux__)
	#define ARC_PLATFORM_LINUX
	#error "Linux is not supported"
#else
	#error "Unknown platform"
#endif

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
