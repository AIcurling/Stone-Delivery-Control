#pragma once
#ifdef WIN32 // Winsock 구 버전 대신 Winsock 2 API 가 사용되는지 확인
	#ifndef _WINSOCKAPI_
	#define _WINSOCKAPI_
	#endif
#endif

#ifdef WIN32 // Windows Headers
	#pragma comment (lib, "ws2_32.lib") // Winsock 라이브러리 자동 링크

	#include <winsock2.h>
	#include <windows.h>
	#include <process.h>

	#define NET_INVALID_SOCKET	INVALID_SOCKET
	#define NET_SOCKET_ERROR SOCKET_ERROR
	typedef int socklen_t; // Unix 타입은 unsigned int Windows 는 int

#else // Unix/Linux Headers
	#include <sys/socket.h> // for socket(), bind(), connect()
	#include <arpa/inet.h> // for sockaddr_in, inet_ntoa()
	#include <pthread.h>
	#include <unistd.h> // for close()
	#include <stdlib.h> // for exit()

	typedef int SOCKET;
	#define NET_INVALID_SOCKET	-1
	#define NET_SOCKET_ERROR -1
#endif

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>