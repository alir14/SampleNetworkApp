#include <iostream>
#include "WindowUDPClient.h"
#include "LinuxUDPClient.h"


void main(int argc, char* argv[]) //int argc, char *argv[]
{
	std::cout << "There are " << argc << " arguments:\n";

	// Loop through each argument and print its number and value
	for (int count{ 0 }; count < argc; ++count)
		std::cout << count << " " << argv[count] << '\n';

	if (argv[1] != NULL && argv[2] != NULL) {
		std::cout << "Type: " << argv[1] << std::endl;
		if (*argv[1] == 'w') {
			WindowUDPClient w_udpClient;

			w_udpClient.RunClient(argv[2]);
		}
		else
		{
			LinuxUDPClient l_udpClient;

			l_udpClient.RunClient(argv[2]);
		}
	}
	
}