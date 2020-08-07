#include "TCPServer.h"

int main()
{
	std::cout << "running server ..." << std::endl;

	TCPServer tcpServer;

	tcpServer.RunServer(55000);
}
