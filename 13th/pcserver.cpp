# include "header.h"
# include <stdlib.h>
# include <pthread.h>
# include <errno.h>
# define LISTEN_QUEUE_SIZE 10
# define THREAD_NUMBER 4

struct queue{
	int queue[THREAD_NUMBER];
	int writeIndex;
	int readIndex;
	int count;
} sharedQueue;

//뮤텍스와 컨디션을 초기화 하시오.
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

void initializeQueue() {
	sharedQueue.readIndex = 0;
	sharedQueue.writeIndex = 0;
	sharedQueue.count = 0;
}

void mutexDestroy() {
	//뮤텍스와 컨디션을 제거하시오.
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&full);
	pthread_cond_destroy(&empty);
}

void *workerTask(void *argument) 
{
	int connectFD;
	int integer1, integer2, sum;
	int readBytes, writtenBytes;
	char sendBuffer[BUFFER_SIZE];
	char receiveBuffer[BUFFER_SIZE];
	while (true) 
	{	
		//프로듀서가 제공한 connect FD를 이용해 클라이언트와 통신하시오.
		pthread_mutex_lock(&mutex);
		while(sharedQueue.count==0)
			pthread_cond_wait(&empty,&mutex);
		connectFD = sharedQueue.queue[sharedQueue.readIndex];
		sharedQueue.readIndex = (sharedQueue.readIndex+1)%4;
		sharedQueue.count--;
		pthread_cond_signal(&full);
		pthread_mutex_unlock(&mutex);
		while(1)
		{
			readBytes = readvn(connectFD, receiveBuffer, BUFFER_SIZE);
			if(readBytes>0)
			{
				receiveBuffer[readBytes]='\0';
				integer1= atoi(receiveBuffer);
				printf("%d\n",integer1);
				fflush(stdout);
			}
			else
				close(connectFD);
			
			readBytes = readvn(connectFD, receiveBuffer, BUFFER_SIZE);
			if(readBytes>0)
			{
				receiveBuffer[readBytes]='\0';
				integer2= atoi(receiveBuffer);
				printf("%d\n",integer2);
				fflush(stdout);
			}
			else
				close(connectFD);
		
			sum = integer1 + integer2;
			sprintf(sendBuffer, "%d", sum);
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
	
	int listenFD = socket(AF_INET, SOCK_STREAM, 0);
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
	
	struct sockaddr_in connectSocket;
	socklen_t connectSocketLength = sizeof(connectSocket);
	int connectFD;
	initializeQueue();
	pthread_t workers[THREAD_NUMBER] = {0};
	int threadNumber;
	
	for (threadNumber = 0; threadNumber < THREAD_NUMBER; threadNumber++) 
	{
		int threadCreateResult;
		threadCreateResult = pthread_create(&workers[threadNumber], NULL, workerTask, NULL);
		if (threadCreateResult != 0) {
			printf("Thread create error\n");
			fflush(stdout);
			break;
		}
	}
	
	
	while (true) {
		connectFD = accept(listenFD, (struct sockaddr*)&connectSocket, &connectSocketLength);
		//connect FD를 공유 큐에 추가하여 컨슈머가 사용할 수 있도록 하시오i.
		pthread_mutex_lock(&mutex);

		while(sharedQueue.count == 4)
		{
			pthread_cond_wait(&full,&mutex);
		}
		sharedQueue.queue[sharedQueue.writeIndex]=connectFD;
		sharedQueue.count++;
		sharedQueue.writeIndex= (sharedQueue.writeIndex+1)%4;
		
		pthread_cond_signal(&empty);
		pthread_mutex_unlock(&mutex);
	}
	
	for (int i = 0; i < threadNumber; i++) {
		pthread_join(workers[i], NULL);
	}
	
	close(listenFD);
	mutexDestroy();
	return 0;
}

