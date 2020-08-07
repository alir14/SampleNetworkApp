#include "TCPClient.h"

int main()
{
	std::cout << "client is started " << std::endl;

	TCPClient tcpClient;

	tcpClient.RunClient("127.0.0.1", 55000);
}
