#include "TCPServer.h"

void TCPServer::RunServer(USHORT port)
{
	SOCKET srvSocket = initializeSocket();

	if (srvSocket != NULL)
	{
		int result = bindAndListeningToSocket(port, srvSocket);

		if (result == 0)
		{
			SOCKET listeningSocket = acceptIncomingMessage(srvSocket);

			processMessage(listeningSocket);
		}
	}
}

SOCKET TCPServer::initializeSocket()
{
	WSADATA wsData;
	WORD version = MAKEWORD(2, 2);

	int wsOk = WSAStartup(version, &wsData);

	if (wsOk != 0)
	{
		std::cout << "cannot initial winsock" << std::endl;
		return NULL;
	}

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);

	if (listening == INVALID_SOCKET)
	{
		std::cout << "cannot create socket" << WSAGetLastError() << std::endl;
		return NULL;
	}

	return listening;
}

int TCPServer::bindAndListeningToSocket(USHORT port, SOCKET& socket)
{
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	int result = bind(socket, (sockaddr*)&hint, sizeof(hint));

	if (result != 0)
	{
		std::cout << "cannot bint to the socket" << WSAGetLastError() << std::endl;
		return result;
	}

	result = listen(socket, SOMAXCONN);

	return result;
}
 
SOCKET TCPServer::acceptIncomingMessage(SOCKET& socket)
{
	// getting info of the connected client
	sockaddr_in clientHint;
	int sizeOfClient = sizeof(clientHint);

	SOCKET clientSocket = accept(socket, (sockaddr*)&clientHint, &sizeOfClient);

	if (clientSocket == SOCKET_ERROR)
	{
		std::cerr << "cannot listen to client" << WSAGetLastError() << std::endl;
		closesocket(socket);
		WSACleanup();
		return NULL;
	}

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&clientHint, sizeof(clientHint), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		std::cout << "connected host: " << host<< "serice: " << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &clientHint.sin_addr, host, NI_MAXHOST);
		std::cout << "connected host: " << host << 
			"serice: " << ntohs(clientHint.sin_port) << std::endl;
	}

	closesocket(socket);

	return clientSocket;
}

void TCPServer::processMessage(SOCKET& socket)
{
	int byteRecv = 0;

	do
	{
		ZeroMemory(buffer, 4096);

		byteRecv = recv(socket, buffer, 4096, 0);

		if (byteRecv == SOCKET_ERROR)
		{
			std::cerr << "Error in recv() " << WSAGetLastError() << std::endl;
			break;
		}

		if (byteRecv == 0)
		{
			std::cout << "client disconnect " << WSAGetLastError() << std::endl;
			break;
		}

		std::cout << std::string(buffer, 0, byteRecv) << std::endl;

		std::string acknowledge = "recevied message (200)";

		send(socket, acknowledge.c_str(), acknowledge.size() + 1, 0);
	} while (true);

	closesocket(socket);

	WSACleanup();
}