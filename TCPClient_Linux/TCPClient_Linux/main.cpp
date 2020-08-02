#include <cstdio>
#include <iostream>
#include "./TCPClient.h"
int main()
{
    std::cout << "client started ..." << std::endl;

    TCPClient _client;

    _client.RunClient(54001);
}