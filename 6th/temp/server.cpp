#include "header.h"
#define MAXLINE 100

int main()
{
	struct sockaddr_in serverSocket, clientSocket;
	memset(&serverSocket, 0, sizeof(serverSocket));
	serverSocket.sin_family = AF_INET;
	serverSocket.sin_addr.s_addr = htonl(INADDR_ANY);
	serverSocket.sin_port = htons(PORT);

	int serverSocketFD = socket(AF_INET, SOCK_DGRAM, 0);

	printf("Waiting for clients.. \n");

	if(bind(serverSocketFD, (struct sockaddr*) &serverSocket, sizeof(serverSocket)) == -1)
	{
		printf("Cannot bind\n");
		return -1;
	}

	int readBytes;
	char sendBuffer[BUFFER_SIZE];
    
	socklen_t connectLength = sizeof(clientSocket);
	while(1)
	{
		//
		readBytes = recvfrom(serverSocketFD, sendBuffer, MAXLINE, 0, (struct sockaddr*)& clientSocket, &connectLength);
		sendto(serverSocketFD, sendBuffer, readBytes, 0, (struct sockaddr*)&clientSocket, connectLength);
	}
	return 0;
}
