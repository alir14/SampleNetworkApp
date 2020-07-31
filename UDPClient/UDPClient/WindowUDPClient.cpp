#pragma once
#include "WindowUDPClient.h"
#include <iostream>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

void WindowUDPClient::RunClient(const char* message) {
	WSADATA data;

	WORD version = MAKEWORD(2, 2);
	int wsOK = WSAStartup(version, &data);
	if (wsOK != 0)
	{
		std::cout << "can't start Winsock! " << wsOK;
		return;
	}

	// create a hint structure for the server
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(54000);
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

	// create Socket 
	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	// write and sent message
	int messageSize = strlen(message);
	std::cout << messageSize << std::endl;

	int sendOk = sendto(out, message, messageSize + 1, 0, (sockaddr*)&server, sizeof(server));

	if (sendOk == SOCKET_ERROR)
	{
		std::cout << "Error, it does not work!!! " << WSAGetLastError() << std::endl;
	}
	// close socket
	closesocket(out);
	//close winsock
	WSACleanup();
}