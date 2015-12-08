# include "header.h"
# include <stdlib.h>
# include <pthread.h>
# define LISTEN_QUEUE_SIZE 10
# define THREAD_NUMBER 4

//뮤텍스를 초기화 하시오.
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void mutexDestroy() {
	//뮤텍스를 제거하시오.
	pthread_mutex_destroy(&mutex);
}

void *workerTask(void *argument) 
{
	int listenFD = (long long) argument;
	struct sockaddr_in connectSocket;
	socklen_t connectSocketLength = sizeof(connectSocket);
	
	int connectFD;
	int integer1, integer2, sum;
	int readBytes, writtenBytes;
	char sendBuffer[BUFFER_SIZE];
	char receiveBuffer[BUFFER_SIZE];

	memset(&sendBuffer,0,BUFFER_SIZE);
	memset(&receiveBuffer,0,BUFFER_SIZE);

	while (true) 
	{
		//뮤텍스를 이용해 accept를 보호하시오
		pthread_mutex_lock(&mutex);
		connectFD = accept(listenFD,(struct sockaddr*)&connectSocket, &connectSocketLength);
		pthread_mutex_unlock(&mutex);

		//클라이언트와 통신하시오.
		while(1)
		{
			readBytes = readvn(connectFD, receiveBuffer, BUFFER_SIZE);
			if(readBytes>0)
			{
				receiveBuffer[readBytes] = '\0';
				integer1 = atoi(receiveBuffer);
				printf("1 %d\n",integer1);
				fflush(stdout);
			}
			else
			{
				close(connectFD);
			}
	
			readBytes = readvn(connectFD, receiveBuffer, BUFFER_SIZE);
			if(readBytes>0)
			{
				receiveBuffer[readBytes] = '\0';
				integer2 = atoi(receiveBuffer);
				printf("2 %d\n",integer2);
				fflush(stdout);
			}
			else
			{
				close(connectFD);
			}
			sum = integer1+integer2;
	
		//printf("%d\n",sum);
		
			sprintf(sendBuffer,"%d",sum);
			writtenBytes = writevn(connectFD, sendBuffer, strlen(sendBuffer));
		}
		close(connectFD);
	}
	return NULL;
}

int main() 
{
	struct sockaddr_in listenSocket;
	memset(&listenSocket, 0, sizeof(listenSocket));
	listenSocket.sin_family = AF_INET;
	listenSocket.sin_addr.s_addr = htonl(INADDR_ANY );
	listenSocket.sin_port = htons(PORT);

	long long listenFD = socket(AF_INET, SOCK_STREAM, 0);
	
	if (bind(listenFD, (struct sockaddr *) &listenSocket, sizeof(listenSocket)) == -1) {
		printf("Can not bind.\n");
		fflush(stdout);
		close(listenFD);
		mutexDestroy();
			return -1;
	}
	if (listen(listenFD, LISTEN_QUEUE_SIZE) == -1) {
		printf("Listen fail.\n");
		fflush(stdout);
		close(listenFD);
		mutexDestroy();
		return -1;
	}
	printf("Waiting for clients...\n");
	fflush(stdout);
	
	pthread_t workers[THREAD_NUMBER];
	
	int threadNumber;
	for (threadNumber = 0; threadNumber < THREAD_NUMBER; threadNumber++) 
	{
		int threadCreateResult;
		threadCreateResult = pthread_create(&workers[threadNumber], NULL, workerTask, (void*)listenFD);
		if (threadCreateResult != 0) {
			printf("Thread create error\n");
			fflush(stdout);
			break;
		}
	}

	for (int i = 0; i < threadNumber; i++) {
		pthread_join(workers[i], NULL);
	}
	
	close(listenFD);
	mutexDestroy();
	return 0;
}

