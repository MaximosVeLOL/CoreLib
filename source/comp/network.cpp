#include <core/comp/network.hpp>

#include <core/platform.hpp>

/*
#if CORE_PLATFORM_DESKTOP

#ifdef _WIN32

#include <windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#endif //win32

#endif //desktop platform

CORE_DECLARE_NAMESPACE

namespace net {

	const char* DEFAULT_PORT = "127.0.0.1";

	COMPONENT_IMPLEMENT_INIT() {
		WSADATA wsaData;

		

		// Initialize Winsock
		s32 iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			return iResult;
		}
	}



	COMPONENT_IMPLEMENT_UNLOAD() {

	}
}

CORE_END_NAMESPACE

*/