#include "header.h"

#define N 5

static struct
{
	int buf;		            
	sem_t full;		
	sem_t empty;
} shared;



void P(sem_t *sem)
{
    if(sem_wait(sem))
		unix_error("P");
}


void V(sem_t *sem)
{    
	if(sem_post(sem))
		unix_error("V");
}



void *thread(void *vargp)
{
//	P(&shared.empty);
	    int myid = *((int*)vargp);
		pthread_detach(pthread_self());
		free(vargp);
//	V(&shared.full);
		    printf("Hello from thread %d\n", myid);
		close(myid);
			return NULL;
}


int main()
{
	pthread_t tid[N];

	sem_init(&shared.empty, 0,1);
	sem_init(&shared.full,0,0);

	for(int i=0; i<N; i++)
	{
		int *addr = (int*)malloc(sizeof(int));
		*addr = i;
		//P(&shared.empty);
		pthread_create(&tid[i],NULL,thread,addr);
		//V(&shared.full);
	}

	/*
	int i =0;
	while(i<N)
	{

		int * conn = (int*)malloc(sizeof(int));
		pthread_create(&tid[i],NULL,thread,conn);
//		P(&shared.full);
		i++;
//		V(&shared.empty);
	}
*/
	for(int i=0; i<N; i++)
		pthread_join(tid[i],NULL);

	exit(0);
}

