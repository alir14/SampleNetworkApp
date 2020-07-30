#include <iostream>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main(int argc, char* argv[]) //int argc, char *argv[]
{

	cout << "There are " << argc << " arguments:\n";

	// Loop through each argument and print its number and value
	for (int count{ 0 }; count < argc; ++count)
		cout << count << " " << argv[count] << '\n';


	WSADATA data;

	WORD version = MAKEWORD(2, 2);
	int wsOK = WSAStartup(version, &data);
	if (wsOK != 0) 
	{
		cout << "can't start Winsock! " << wsOK;
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
	string s(argv[1]);
	int sendOk = sendto(out, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));

	if (sendOk == SOCKET_ERROR) 
	{
		cout << "Error, it does not work!!! " << WSAGetLastError() << endl;
	}
	// close socket
	closesocket(out);
	//close winsock
	WSACleanup();
}