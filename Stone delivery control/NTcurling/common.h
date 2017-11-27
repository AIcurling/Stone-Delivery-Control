#pragma once
#ifdef WIN32 // Winsock �� ���� ��� Winsock 2 API �� ���Ǵ��� Ȯ��
	#ifndef _WINSOCKAPI_
	#define _WINSOCKAPI_
	#endif
#endif

#ifdef WIN32 // Windows Headers
	#pragma comment (lib, "ws2_32.lib") // Winsock ���̺귯�� �ڵ� ��ũ

	#include <winsock2.h>
	#include <windows.h>
	#include <process.h>

	#define NET_INVALID_SOCKET	INVALID_SOCKET
	#define NET_SOCKET_ERROR SOCKET_ERROR
	typedef int socklen_t; // Unix Ÿ���� unsigned int Windows �� int

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