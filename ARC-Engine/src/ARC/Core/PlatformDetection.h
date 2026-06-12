#pragma once

// Platform detection
#ifdef _WIN32
	#ifdef _WIN64
		#define ARC_PLATFORM_WINDOWS
	#else
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "iOS simulator is not supported"
	#elif TARGET_OS_IPHONE == 1
		#define ARC_PLATFORM_IOS
		#error "iOS is not supported"
	#elif TARGET_OS_MAC == 1
		#define ARC_PLATFORM_MACOS
		#error "macOS is not supported"
	#else
		#error "Unknown Apple platform"
	#endif
#elif defined(__ANDROID__)
	#define ARC_PLATFORM_ANDROID
	#error "Android is not supported"
#elif defined(__linux__)
	#define ARC_PLATFORM_LINUX
#else
	#error "Unknown platform"
#endif

// DLL export/import macros (Windows) or empty (Linux/Mac)
#ifdef ARC_PLATFORM_WINDOWS
	#ifdef ARC_DYNAMIC_LINK
		#ifdef ARC_BUILD_DLL
			#define ARC_API  __declspec(dllexport)
		#else
			#define ARC_API  __declspec(dllimport)
		#endif
	#else
		#define ARC_API
	#endif
#else
	#define ARC_API
#endif
