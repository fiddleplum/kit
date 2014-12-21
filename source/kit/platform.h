#pragma once

#ifdef _WIN64
	#define KIT_WINDOWS
#elif _WIN32
	#define KIT_WINDOWS
#elif __APPLE__
	#include "TargetConditionals.h"
	#if TARGET_IPHONE_SIMULATOR
		#include KIT_IOS_SIMULATOR
	#elif TARGET_OS_IPHONE
		#include KIT_IOS
	#elif TARGET_OS_MAC
		#include KIT_OSX
	#endif
#elif __ANDROID__
	#define KIT_ANDROID
#elif __linux
	#define KIT_LINUX
#endif

