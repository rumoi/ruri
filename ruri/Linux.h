#pragma once

#include <climits>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAKEWORD(a,b)

#define WSAStartup(a,b) (1 < 0)

#define SOCKADDR_IN sockaddr_in
#define SOCKADDR sockaddr
#define closesocket(sock) close(sock)
#define ZeroMemory(addr, len) memset(addr, 0, len)
#define _strcmpi(a, b) strcasecmp(a, b)
#define __forceinline 
#define strcpy_s(to, len, from) strncpy(to, from, len)
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),(mode)))==NULL
#define SOCKET_ERROR -1
#define INVALID_SOCKET ((SOCKET)(~0))

//Linux sleeping will be 10x faster
#define Sleep(a) usleep(size_t(double(double(a) / 100.) + 0.5)) 

#define clock_ms std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count

typedef unsigned int DWORD;
typedef unsigned short USHORT;
typedef size_t SOCKET;
typedef void* LPTHREAD_START_ROUTINE;
typedef size_t WSADATA;