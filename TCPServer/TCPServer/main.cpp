#include <iostream>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

void main() 
{
	// initialize winsock
	WSAData wsData;
	WORD version = MAKEWORD(2, 2);

	int wsOK = WSAStartup(version, &wsData);

	if (wsOK != 0) {
		std::cout << "cannot initial winsock" << std::endl;
		return;
	}

	// create socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		std::cout << "cannot create socket" << std::endl;
		return;
	}

	// bind ip address and port to a socket

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(55000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // we can use inet_pton to connet to 127.0.0.1

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// tell winsock the socket is for listening
	listen(listening, SOMAXCONN);

	// wait for connection
	sockaddr_in client;
	int sizeofClient = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &sizeofClient);

	if (clientSocket == INVALID_SOCKET) 
	{
		std::cout << "invalid socket" << std::endl;
	}

	char host[NI_MAXHOST]; // client remote name
	char service[NI_MAXHOST]; // service or port that the client is connect
	// for windows
	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);
	// for linux i can use memset
	//memset(host, 0, NI_MAXHOST);
	//memset(service, 0, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		std::cout << "connected on " << service << std::endl;
	}
	else 
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on port " << 
			ntohs(client.sin_port) << std::endl;
	}

	// close listening socket
	closesocket(listening);

	// loop : accept and echo message back to client
	int bytesReceived;
	char buf[4096];

	while (true)
	{
		//memset(buf, 0, 4096);
		ZeroMemory(buf, 4096);
		//wait for client to sed data
		bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			std::cerr << "Error in recv()" << std::endl;
			break;
		}
		// echo message back
		if (bytesReceived == 0)
		{
			std::cout << "client disconnect" << std::endl;
			break;
		}

		send(clientSocket, buf, bytesReceived + 1, 0);
	}
	
	//close socket
	closesocket(clientSocket);

	// cleanup winsok
	WSACleanup();
}