

#include "header.h"

# define LISTEN_QUEUE_SIZE 5

int main()
{
	// listen socket data structure
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr = inet_addr(argv[0]);


	// create listen file descriptor

	// bind and listen

	printf("Waiting for clients...\n");

	while(1)
	{
		// define communicate socket

		// permit client request

		char buffer[BUFFER_SIZE] = {0};
		sprintf(buffer,"Hellow\n");
		write(connectFD,buffer,strlen(buffer));

		// close client connection
		connectFD.close();
	}

	// close listen socket
	listenFD.close();

	return 0;
}
