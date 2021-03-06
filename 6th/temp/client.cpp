#include "header.h"

#define TIMEOUT 5


void alarmHandler(int sig)
{ }

void signal_handler(int sig)
{ }

int main(int argc, char ** argv)
{
	signal(SIGALRM, alarmHandler);

	// make signal action variable
	struct sigaction act;
	act.sa_handler = signal_handler;
	sigaction(SIGALRM, &act ,NULL);

	if(argc != 2)
	{
		printf("Usage: %s IPv4-address\n", argv[0]);
		return -1;
	}

	struct sockaddr_in serverSocket;
	memset(&serverSocket, 0, sizeof(serverSocket));
	serverSocket.sin_family = AF_INET;
	inet_aton(argv[1],(struct in_addr*)&serverSocket.sin_addr.s_addr);
	serverSocket.sin_port = htons(PORT);
	
	int clientSocketFD = socket(AF_INET , SOCK_DGRAM, 0);

	int readBytes;
	char sendBuffer[BUFFER_SIZE];
	char receiveBuffer[BUFFER_SIZE];

	
	while(true)
	{
		// put string and send to server
		scanf("%s",sendBuffer);
		sendto(clientSocketFD, sendBuffer, strlen(sendBuffer), 0,(struct sockaddr*)&serverSocket, sizeof(serverSocket));
		
		// waiting until timeout 
		alarm(TIMEOUT);
		if(( readBytes = recvfrom(clientSocketFD, receiveBuffer, BUFFER_SIZE, 0, NULL,NULL))<0)
		{
			if(errno ==EINTR)	
				fprintf(stderr,"socket timeout\n");
			else
				err_sys("recvfrom error");
		}
		// receive string from server
		else
		{
			alarm(0);
			receiveBuffer[readBytes] = 0;
			fputs(receiveBuffer, stdout);
			printf("\n");
		}
	}
	return 0;
}
