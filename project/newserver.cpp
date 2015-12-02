# include "header.h"
# include <pthread.h>

# define PUB 3
# define SUB 3

struct MSGA msgA;
struct MSGB msgB;
struct MSGC msgC;

int Rtopics[3]={0}; // write topic variable
int Stopics[3]={0};	// read topic variable
int pub =0;
int sub =0;

void* clientFunc(void* arguments)
{
	int connfd = *((int*)arguments);
	pthread_detach(pthread_self());
	free(arguments);

	char Rbuffer[BUFFER_SIZE];
	int n = readvn(connfd, Rbuffer, BUFFER_SIZE);

//	msgA = (struct MSGA)Rbuffer;

	memcpy(&msgA,Rbuffer,sizeof(msgA));

	if(n>0)
	{
		//Rbuffer[n]='\0';
		printf("%s\n",Rbuffer);
		printf("%s\n",msgA.id);
		printf("%s\n",msgA.msg);
	}

	close(connfd);
}

int main(int argc, char** argv)
{
	/** initialize msg **/
	msgA = initA(msgA);
	msgB = initB(msgB);
	msgC = initC(msgC);
	
	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	int listenFD = socket(AF_INET, SOCK_STREAM, 0);
	if(bind(listenFD, (struct sockaddr *)&servaddr, sizeof(servaddr))==-1)
	{
		printf("Cannot bind.\n");
		return -1;
	}

	if(listen(listenFD, BUFFER_SIZE) == -1)
	{
		printf("Listen fail.\n");
		return -1;
	}

	printf("Waiting for clients...\n");

	while(1)
	{
		struct sockaddr_in caddr;
		socklen_t clen= sizeof(caddr);

		pthread_t tid;

		int *connfd = (int *)malloc(sizeof(int));
		*connfd = accept(listenFD, (struct sockaddr*)&caddr, &clen);
			
		pthread_create(&tid, NULL, clientFunc, connfd);
	}
	return 0;

}
