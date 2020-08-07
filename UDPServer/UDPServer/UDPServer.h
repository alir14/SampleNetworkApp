#pragma once
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class UDPServer
{
public:
	void RunServer();
private:
	int initializeWinSocket();
	SOCKET createAndBinSocket(int port);
	void listeningAndProcess(SOCKET& in);
	void listeningAndProcessAndResponse(int port, SOCKET& in);
	void sendAcknowledgeToClientMessage(int port, const char* clinetIP);
};

