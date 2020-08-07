#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

class TCPServer
{
	char buffer[4096];
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
public:
	void RunServer(USHORT port);
private:
	SOCKET initializeSocket();
	int bindAndListeningToSocket(USHORT port, SOCKET& socket);
	SOCKET acceptIncomingMessage(SOCKET& socket);
	void processMessage(SOCKET& socket);

};

