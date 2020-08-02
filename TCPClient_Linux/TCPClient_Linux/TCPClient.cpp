#include "TCPClient.h"

void TCPClient::RunClient(uint16_t portNumber)
{
	//create a socket
	int listeningSocket = createListeningSocket();

	if (listeningSocket > 0)
	{
		int result = bindSocketToNetworkAndListen(portNumber, listeningSocket);

		if (result > 0)
		{
			int clientSocket = acceptMessage(listeningSocket);

			if (clientSocket > 0)
				processMessage(clientSocket);
		}
	}
}

int TCPClient::createListeningSocket()
{
	int listening = socket(AF_INET, SOCK_STREAM, 0);

	if (listening == -1)
	{
		std::cerr << "cannot create a scoket";
		return -1;
	}

	return listening;
}

int TCPClient::bindSocketToNetworkAndListen(uint16_t port, int listeningSocket)
{
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

	if (bind(listeningSocket, (sockaddr*)&hint, sizeof(hint)) == -1)
	{
		std::cerr << "cannot bind to IP/PORT";

		return -2;
	}

	if (listen(listeningSocket, SOMAXCONN) == -1) {
		std::cerr << "cannot listen anymore !!!";
		return -3;
	}

	return 1;
}

int TCPClient::acceptMessage(int listeningSocket)
{
	sockaddr_in client;
	socklen_t clientSize = sizeof(client);

	int clientSocket = accept(listeningSocket, (sockaddr*)&client, &clientSize);

	if (clientSocket == -1)
	{
		std::cerr << "Problem with client connection";
		return -4;
	}

	char hostName[NI_MAXHOST];
	char port[NI_MAXSERV];

	close(listeningSocket);

	memset(hostName, 0, NI_MAXHOST);
	memset(port, 0, NI_MAXSERV);

	int result = getnameinfo((sockaddr*)&client, sizeof(client), hostName, NI_MAXHOST, port, NI_MAXSERV, 0);

	if (result == 0)
	{
		std::cout << hostName << "connected on" << port << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, hostName, NI_MAXHOST);
		std::cout << hostName << "connected on" << ntohs(client.sin_port) << std::endl;
	}

	return clientSocket;
}

void TCPClient::processMessage(int clientSocket)
{
	char buff[4096];

	while (true)
	{
		memset(buff, 0, 4096);

		ssize_t byteReceived = recv(clientSocket, buff, 4096, 0);

		if (byteReceived == -1)
		{
			std::cerr << "there is a connection issue with client" << std::endl;
			break;
		}

		if (byteReceived == 0)
		{
			std::cerr << "client disconnected" << std::endl;
			break;
		}

		std::cout << "Received: " << std::string(buff, 0, byteReceived) << std::endl;

		//process or echo back
		send(clientSocket, buff, byteReceived + 1, 0);
	}

	close(clientSocket);
}

