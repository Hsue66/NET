#include "header.h"

#define LISTEN_QUEUE_SIZE 5

int main(){

	//socket data structure
/*	struct sockaddr_in
	{
		unsigned short sin_family;
		unsigned short sin_port;
		struct in_addr sin_addr;
		unsigned char sin_zero[8];
	};
*/

	// create listen file descriptor
	int listen_socket;
	listen_socket = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == listen_socket)
	{
		printf("Listen socket Creation Failure\n");
		exit(1);
	}


	// bind
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(-1 == bind(listen_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))
	{
		printf("Bind Failure\n");
		exit(1);
	}


	// listen
	if( -1 == listen(listen_socket ,5))
	{
		printf("Listen Failure\n");
		exit(1);
	}

	printf("Waiting for clinent...\n");

	//---- client struct
	struct sockaddr_in connect_addr;
//	int connect_size;
	int connectFD;

	socklen_t connect_size;

	while(1)
	{
		// define communication socket
		connect_size = sizeof(connect_addr);

		// Connection request
		connectFD = accept(listen_socket, (struct sockaddr*)&connect_addr, &connect_size);
		if(-1 == connectFD)
		{
			printf("Client connect Failure\n");
			exit(1);
		}

		
		char buffer[BUFFER_SIZE] = {0};
		sprintf(buffer,"Hello\n");
		write(connectFD, buffer, strlen(buffer));
	
		//close
		close(connectFD);
	}

	//close listen
	close(listen_socket);


	return 0;
}
