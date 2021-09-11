#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

void error(const char* msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char* argv[])
{
    int sockfd, portNo;
    struct sockaddr_in server_addr;
    struct hostent* server;

    char buffer[256];

    portNo = 54000; //atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname("192.168.1.67");

    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    
    inet_pton(AF_INET, "192.168.1.79", &server_addr.sin_addr);

    server_addr.sin_port = htons(portNo);

    if(connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        error("ERROR connecting");

    printf("Please enter the message: ");
    bzero(buffer, 256);
    fgets(buffer, 256, stdin);

    int msgSize = strlen(buffer) + 1;
    printf("message size: %i\n", msgSize);

    int sendOk = sendto(sockfd, buffer, msgSize, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    close(sockfd);

    return 0;
}

