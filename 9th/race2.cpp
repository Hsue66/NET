#include "header.h"

#define N 1000

void *thread(void *vargp)
{
	int myid = *((int *)vargp);
	free(vargp);
	printf("Hello from thread %d\n", myid);
	return NULL;
}


int main()
{
	pthread_t tid[N];

	for(int i=0; i<N; i++)
	{
		int *addr = (int*)malloc(sizeof(int));
		*addr = i;
		pthread_create(&tid[i],NULL,thread,addr);
	}

	for(int i=0; i<N; i++)
		pthread_join(tid[i],NULL);

	exit(0);
}

