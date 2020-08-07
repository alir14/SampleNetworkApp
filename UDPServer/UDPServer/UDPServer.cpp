#include<iostream>
#include "UDPServer.h"

void UDPServer::RunServer()
{
	//start up winsock
	int result = initializeWinSocket();

	if (result > 0) {
		// Bind socket to ip and port
		SOCKET listening = createAndBinSocket(54000);
		if (listening != NULL) 
		{
			// client sender metadata
			//listeningAndProcess(listening);
			listeningAndProcessAndResponse(54010, listening);
		}
		// Shutdown winsock
		closesocket(listening);
	}

	WSACleanup();
}

int UDPServer::initializeWinSocket()
{
	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	int wsOk = WSAStartup(version, &data);

	if (wsOk != 0)
	{
		std::cout << "cannot start Winsock" << wsOk;
		return -1;
	}

	return 1;
}

SOCKET UDPServer::createAndBinSocket(int port)
{
	// Bind socket to ip and port
	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(port);

	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		std::cout << "cannot binf socket" << WSAGetLastError() << std::endl;
		return NULL;
	}

	return in;
}

void UDPServer::listeningAndProcess(SOCKET &inSocket)
{
	sockaddr_in client;
	int clientLength = sizeof(client);
	ZeroMemory(&client, clientLength);

	char buf[1024];

	//enter a loop
	while (true)
	{
		ZeroMemory(buf, 1024);
		// wait for message
		int bytesIn = recvfrom(inSocket, buf, 1024, 0, (sockaddr*)&client, &clientLength);
		if (bytesIn == SOCKET_ERROR) {
			std::cout << "Error receiving from cleint " << WSAGetLastError() << std::endl;
			continue;
		}

		// display message and client info
		char clientIp[256];
		ZeroMemory(clientIp, 256);

		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		std::cout << "message received from " << clientIp << " : " << buf << std::endl;
	}
}

void UDPServer::listeningAndProcessAndResponse(int port, SOCKET& in)
{
	sockaddr_in client;
	int clientLength = sizeof(client);
	ZeroMemory(&client, clientLength);


	char buff[1024];

	//enter a loop
	while (true)
	{
		ZeroMemory(buff, 1024);
		// wait for message
		int bytesIn = recvfrom(in, buff, 1024, 0, (sockaddr*)&client, &clientLength);
		if (bytesIn == SOCKET_ERROR) {
			std::cout << "Error receiving from cleint " << WSAGetLastError() << std::endl;
			continue;
		}

		// display message and client info
		char clientIp[256];
		ZeroMemory(clientIp, 256);

		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		std::cout << "message received from " << clientIp << " : " << buff << std::endl;

		sendAcknowledgeToClientMessage(port, clientIp);
	}
}

void UDPServer::sendAcknowledgeToClientMessage(int port, const char* clinetIP)
{
	const std::string acknowledge = "ok";

	sockaddr_in senderHint;
	senderHint.sin_family = AF_INET;
	senderHint.sin_port = htons(port);
	inet_pton(AF_INET, clinetIP, &senderHint.sin_addr);

	SOCKET sendSocket = socket(AF_INET, SOCK_DGRAM, 0);

	int sendResult = sendto(sendSocket, acknowledge.c_str(), acknowledge.size(), 0, (sockaddr*)&senderHint, sizeof(senderHint));
	if (sendResult == SOCKET_ERROR)
	{
		std::cerr << "Error, it does not work!!! " << WSAGetLastError() << std::endl;
	}

	closesocket(sendSocket);
}