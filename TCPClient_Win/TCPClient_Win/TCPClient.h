#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

class TCPClient
{
	char buffer[4096];
public:
	void RunClient(const char* serverIP, USHORT port);
private:
	SOCKET createSocket();
	int connectToSertver(const char* serverIP, USHORT port, SOCKET& socket);
	void processMessage(SOCKET& socket);
};
