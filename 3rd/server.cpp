#include "header.h"

#define LISTEN_QUEUE_SIZE 5

int main()
{
	struct sockaddr_in listenSocket;

////////////////////////////////////////////////////
	// listen socket set
	memset(&listenSocket, 0, sizeof(listenSocket));
	listenSocket.sin_family = AF_INET;
	listenSocket.sin_port = PORT;
	listenSocket.sin_addr.s_addr = htonl(INADDR_ANY);

//////////////////////////////////////////////////////
	int listenFD = socket(AF_INET, SOCK_STREAM, 0);

	if(bind(listenFD, (struct sockaddr *) &listenSocket, sizeof(listenSocket)) == -1)
	{
		printf("Cannot bind\n");
		return -1;
	}

	if(listen(listenFD, LISTEN_QUEUE_SIZE) == -1)
	{
		printf("Listen fail\n");
		return -1;
	}

	printf("Waiting for clients..\n");

	while(1)
	{
		struct sockaddr_in connectSocket, peerSocket;
		socklen_t connectSocketLength = sizeof(connectSocket);
		
///////////////////////////////////////////////////////
		// Client access permission socket set
		int connectFD;
		connectFD = accept(listenFD, (struct sockaddr*)&connectSocket, &connectSocketLength);
		if(-1 == connectFD)
		{
			printf("Cannot accept\n");
			return -1;
		}

////////////////////////////////////////////////////////////

		getpeername(connectFD, (struct sockaddr*)&peerSocket, &connectSocketLength);
		char peerName[sizeof(peerSocket.sin_addr) +1] = {0};
		sprintf(peerName, "%s", inet_ntoa(peerSocket.sin_addr));

		printf("Client: %s\n", peerName);
	
		char buffer[BUFFER_SIZE] = {0};
		sprintf(buffer, "Hello, %s\n", peerName);
		writevn(connectFD, buffer, strlen(buffer));

		close(connectFD);
	}
	close(listenFD);
	return 0;

}
