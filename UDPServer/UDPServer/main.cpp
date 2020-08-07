#pragma once
#include<iostream>
#include "UDPServer.h"

void main() 
{
	std::cout << "starting ..." << std::endl;

	UDPServer udpServer;

	udpServer.RunServer();
}
