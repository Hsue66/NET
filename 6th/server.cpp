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
#define N 4
#define MAXLINE 100

int count =0;

void childHandler(int signal)
{
	//
	int child_status;
	pid_t pid;
	while((pid = waitpid(-1, &child_status,WNOHANG)) > 0)
	{
		count --;
		printf("Received signal %d from process %d\n",signal,pid);
	}
}

void echo(int connfd)
{
	size_t n;
	char buf[MAXLINE];
//	rio_t rio;

//	rio_readinitb(&rio, connfd);
	n=read(connfd, buf, sizeof(buf));
	while(n != 0 )
	{
		write(connfd, buf,sizeof(buf));
	}

}

int main()
{
	//
	signal(SIGCHLD, childHandler);
	pid_t pid[N];

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
		
		//
		count= N;
		for ( int i =0; i<count; i++)
		{
			if((pid[i] =fork()) == 0)
			{
				close(listenFD);
				echo(connectFD);
				close(connectFD);
				exit(0);
			}
		}
		close(connectFD);
	}
	close(listenFD);

	return 0;

}

