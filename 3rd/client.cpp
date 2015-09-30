#include "header.h"


int main(int argc, char** argv)
{

	if(argc != 2)
	{
		printf("Usage: %s IPv4-address \n",argv[0]);
		return -1;
	}

	struct sockaddr_in connectSocket;
	
////////////////////////////////////////
	//client socket set
	memset(&connectSocket, 0, sizeof(connectSocket));
	connectSocket.sin_family = AF_INET;
	connectSocket.sin_port = PORT;
	connectSocket.sin_addr.s_addr = inet_addr(argv[1]);
//////////////////////////////////////

	int connectFD = socket(AF_INET, SOCK_STREAM, 0);
	
	if(connect(connectFD, (struct sockaddr*) &connectSocket, sizeof(connectSocket)) == -1)
	{
		printf("Cannot connect\n");
		return -1;
	}
	else
	{
		char buffer[BUFFER_SIZE];
		int n = readvn(connectFD, buffer, BUFFER_SIZE);
		printf("%d bytes read\n",n);
	
		buffer[n] = '\0';
		fputs(buffer, stdout);
		fflush(stdout);
	}
	close(connectFD);
	return 0;

}
