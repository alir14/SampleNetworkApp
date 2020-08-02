#pragma once
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

class TCPClient
{
public:
	void RunClient(uint16_t portNumber);
private:
	int createListeningSocket();
	int bindSocketToNetworkAndListen(uint16_t port, int listeningSocket);
	int acceptMessage(int listeningSocket);
	void processMessage(int clientSocket);
};
