#include "TCPClient.h"

void TCPClient::RunClient(const char* serverIP, USHORT port)
{
	SOCKET tcpClientScoket = createSocket();

	if (tcpClientScoket != NULL)
	{
		int result = connectToSertver(serverIP, port, tcpClientScoket);

		if (result == 0)
		{
			processMessage(tcpClientScoket);
		}
	}
}

SOCKET TCPClient::createSocket()
{
	WSAData wsData;
	WORD version = MAKEWORD(2, 2);

	int wsOk = WSAStartup(version, &wsData);

	if (wsOk != 0) 
	{
		std::cerr << "cannot initial winsocket" << std::endl;
		return NULL;
	}

	// create a socket
	SOCKET senderSocket = socket(AF_INET, SOCK_STREAM, 0);
	
	if (senderSocket == INVALID_SOCKET)
	{
		std::cerr << "cannot create a scoekt" << WSAGetLastError() << std::endl;
		WSACleanup();
		return NULL;
	}

	return senderSocket;
}

int TCPClient::connectToSertver(const char* serverIP, USHORT port, SOCKET& socket)
{
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, serverIP, &hint.sin_addr);

	int result = connect(socket, (sockaddr*)&hint, sizeof(hint));

	if (result == -1)
	{
		std::cerr << "cannot connect to the scoekt" << WSAGetLastError() << std::endl;
		closesocket(socket);
		WSACleanup();
	}

	return result;
}

void TCPClient::processMessage(SOCKET& socket)
{
	std::string userInput;

	do
	{
		std::cout << "enter message > " << std::endl;

		std::getline(std::cin, userInput);

		if (userInput.size() > 0)
		{
			int sendResult = send(socket, userInput.c_str(), userInput.size() + 1, 0);

			if (sendResult != SOCKET_ERROR)
			{
				ZeroMemory(buffer, 4096);
				int byteRecv = recv(socket, buffer, 4096, 0);

				if (byteRecv > 0)
				{
					//show response
					std::cout << "destination> " << std::string(buffer, 0, byteRecv) << std::endl;
				}
			}
		}
	} while (userInput != ":q");

	closesocket(socket);

	WSACleanup();
}