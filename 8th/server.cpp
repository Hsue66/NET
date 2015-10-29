#include "header.h"
#include <pthread.h>
//pthread를 위한 헤더를 추가하시오

void* workerThreadFunction(void *arguments)
{
	//readvn과 writevn을 이용해 클라이언트가 보낸 랜덤 넘버를 더해 되돌려보낸다
	//클라이언트 ID를 포함해 받은 메시지와 두 랜덤 넘버의 합을 출력하시오
	
	int connectFD = *((int*)arguments);
	pthread_detach(pthread_self());
	free(arguments);

	char readBuffer1[BUFFER_SIZE],readBuffer2[BUFFER_SIZE], receiveBuffer[BUFFER_SIZE];

	int n = readvn(connectFD, readBuffer1,BUFFER_SIZE);
	n = readvn(connectFD, readBuffer2,BUFFER_SIZE);

	sprintf(receiveBuffer,"%d",atoi(readBuffer1)+atoi(readBuffer2));
	writevn(connectFD, receiveBuffer, strlen(receiveBuffer));

	fflush(stdout);
	printf("received %s %s | send %s\n",readBuffer1,readBuffer2,receiveBuffer);	 
	
	fflush(stdout);

	close(connectFD);
}

int main()
{
	//클라이언트가 접속하면 쓰레드를 새로 생성하여 
	//위 workerThreadFunction를 실행하도록 하시오

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
		struct sockaddr_in clientaddr;

		socklen_t  clientlen=sizeof(clientaddr);
		pthread_t tid;

		int *connectFD = (int *)(malloc(sizeof(int)));
		*connectFD = accept(listenFD, (struct sockaddr *)&clientaddr, &clientlen);
		
		pthread_create(&tid, NULL, workerThreadFunction, connectFD);
	}
	
	return 0;
}
