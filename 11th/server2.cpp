# include "header.h"

int main()
{
	struct sockaddr_in listenSocket;
	memset(&listenSocket, 0 , sizeof(listenSocket));
	listenSocket.sin_family = AF_INET;
	listenSocket.sin_addr.s_addr = htonl(INADDR_ANY);
	listenSocket.sin_port = htons(PORT);

	int listenFD = socket(AF_INET, SOCK_STREAM, 0);
	
	if(bind(listenFD, (struct sockaddr *)&listenSocket, sizeof(listenSocket))==-1)
	{
		printf("Can not bind,\n");
		return -1;
	}							
	
	if(listen(listenFD,BUFFER_SIZE) == -1)
	{
		printf("Listen fail.\n");
		return -1;
	}

	printf("Waiting for clients..\n");

	while(1)
	{
		struct sockaddr_in connectSocket, peerSocket;
		socklen_t connectSocketLength = sizeof(connectSocket);
		
	    int	connectFD = accept(listenFD, (struct sockaddr*)&connectSocket, &connectSocketLength);
		if(-1 == connectFD)
		{
			printf("Cannot accept\n");
			return -1;
		}
		
//		getpeername(connectFD, (struct sockaddr*)&peerSocket, &connectSocketLength);
//		char peerName[sizeof(peerSocket.sin_addr) +1] = {0};
//		sprintf(peerName, "%s", inet_ntoa(peerSocket.sin_addr));

		char readBuffer[BUFFER_SIZE], readBuffer2[BUFFER_SIZE];
		int r = readvn(connectFD,readBuffer, BUFFER_SIZE);
		readBuffer[r] = '\0';
		
//		int s =readvn(connectFD,readBuffer2, BUFFER_SIZE);
//		readBuffer2[s] = '\0';
		
		printf("%d, %s\n",r,readBuffer);
		fflush(stdout);
		
		char writeBuffer[BUFFER_SIZE];
		sprintf(writeBuffer, "%s", readBuffer);
		writevn(connectFD, writeBuffer, strlen(writeBuffer));

		close(connectFD);
	}
	
	close(listenFD);

	return 0;
}
