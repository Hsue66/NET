#include "header.h"

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		printf("Usage: %s IPv4-address\n",argv[0]);
		return -1;
	}

	// DEFINE  server socket data structure
	int connectFD;
	struct sockaddr_in server_addr;
	

	// DEFINE connect file descriptor
	connectFD = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == connectFD)
	{
		printf("Creation Failure\n");
		exit(1);
	}	

//	char addr[100];
//	printf("INPUT addr: ");
//	scanf("%s",addr);

	memset(&server_addr, 0 , sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);


	// connect to server
	if(-1==connect(connectFD, (struct sockaddr*)&server_addr, sizeof(server_addr)))
	{
		printf("Connection Failure\n");
		exit(1);
	}

	char buffer[BUFFER_SIZE];
	int n = read(connectFD, buffer, BUFFER_SIZE);
	printf("%d bytes read\n",n);

	buffer[n] = '\0';
	fputs(buffer, stdout);
	fflush(stdout);


	// CLOSE 
	close(connectFD);

	return 0;
}
