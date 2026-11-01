#ifndef __PLATFORM_HPP__
#define __PLATFORM_HPP__

//platform.hpp - Platform specific stuff, not defined here btw.


enum EPlatform : unsigned __int8 {
	//SDL supported platforms:
	// All Desktops (Windows, Linux, MacOS, ect.)
	// Web (Emscripten)
	// Mobile (Android, iOS, ect.)
	//Due to SDL3 supported all those platforms, we don't have to write the complicated stuff for those platforms, just use SDL!
	PLATFORM_SDLSUPPORTED = 0, 
	PLATFORM_PSX,
	PLATFORM_GBA,
	PLATFORM_WII,
	PLATFORM_WIIU,
	PLATFORM_UNKNOWN
}; 

#if defined(WIN32) || defined(__APPLE__) && defined(__MACH__) || defined( __linux__) || defined(__MINGW64__)

#define CORE_PLATFORM_DESKTOP 1

#endif

//If we are an SDL supported platform, set to SDL supported, or else we need to set it automatically
#if defined(WIN32) || defined(__EMSCRIPTEN) || defined(ANDROID) 

#define CORE_PLATFORM PLATFORM_SDLSUPPORTED
#define CORE_PLATFORM_MODERN 1

#else

#define CORE_PLATFORM PLATFORM_UNKNOWN

#endif

#endif