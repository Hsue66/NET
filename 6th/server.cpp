#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>

#define PORT 10001
#define LISTEN_QUEUE_SIZE 5
#define BUFFER_SIZE 100

void childHandler(int signal)
{
	// when client die, child process die
	pid_t pid;
	while((pid = waitpid(-1, 0,WNOHANG)) > 0)
	{
		printf("Received signal %d from process %d\n",signal,pid);
	}
}

int main()
{
	// catch child signal
	signal(SIGCHLD, childHandler);

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

	if(listen(listenFD,LISTEN_QUEUE_SIZE) == -1)
	{
		printf("Listen fail.\n");
		return -1;
	}

	printf("Waiting for clients..\n");

	while(1)
	{
		struct sockaddr_in connectSocket, peerSocket;
		socklen_t connectSocketLength = sizeof(connectSocket);
		int connectFD = accept(listenFD, (struct sockaddr * )&peerSocket, &connectSocketLength);
		char peerName[sizeof(peerSocket.sin_addr) +1] = {0};
		sprintf(peerName, "%s", inet_ntoa(peerSocket.sin_addr));

		printf("Client: %s \n", peerName);
		
		// when client connected, fork to make child process and do echo
		char str[BUFFER_SIZE];
		int str_len;

		if((fork()) == 0)
		{
			close(listenFD);
			while((str_len = read(connectFD, str, BUFFER_SIZE))>0)
			{
				write(connectFD, str, str_len);
			}

			close(connectFD);
			exit(0);
		}
		close(connectFD);
	}
	close(listenFD);

	return 0;

}

