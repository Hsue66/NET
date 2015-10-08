#include "header.h"

#define TIMEOUT 5
#define MAXLINE 100
struct sigaction act;
struct sigaction actol;

void alarmHandler(int sig)
{
	;
}

void signal_handler(int sig)
{
	printf("5초지남\n");
	sigaction(SIGALRM,&actol,NULL);
}

int main(int argc, char ** argv)
{
	signal(SIGALRM, alarmHandler);

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

	act.sa_handler = signal_handler;
	sigemptyset(&act.sa_mask);
	
				sigaction(SIGALRM, &act ,&actol);
	while(true)
	{
		//
		scanf("%s",sendBuffer);

		//
		sendto(clientSocketFD, sendBuffer, strlen(sendBuffer), 0,(struct sockaddr*)&serverSocket, sizeof(serverSocket));
		//
		alarm(5);
		if(( readBytes = recvfrom(clientSocketFD, receiveBuffer, MAXLINE, 0, NULL,NULL))<0)
		{
			if(errno ==EINTR)	
			{
				printf("not");
				sigaction(SIGALRM, &act ,&actol);
			}
			else
				err_sys("recvfrom error");
		}else
		{
			alarm(0);
			receiveBuffer[readBytes] = 0;
			fputs(receiveBuffer, stdout);
		}
	}
	return 0;
}
