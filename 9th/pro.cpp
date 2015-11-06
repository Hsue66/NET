#include "header.h"

#define NITERS 5
static struct
{
	int buf;
	sem_t full;
	sem_t empty;
} shared, wshared;



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


void *producer(void *arg)
{
	int i, item;
	for(i=0; i<NITERS; i++)
	{
		item = i;

		P(&shared.empty);	
		shared.buf = item;
		printf("Produced %d \n",item);
		V(&shared.full);
	}
	return NULL;
}

void *consumer(void *arg)
{
	int i, item;
	for(i=0; i<NITERS; i++)
	{
		P(&shared.full);
		item = shared.buf;
		V(&shared.empty);

		printf("consumed %d\n",item);
		
		P(&wshared.empty);
		item++;
		shared.buf = item;
		V(&wshared.full);

		printf("produced %d by consumer \n",item);
	}
	return NULL;
}


int main()
{
	pthread_t tid_producer;
	pthread_t tid_consumer;


	sem_init(&shared.empty, 0,1);
	sem_init(&shared.full, 0,0);

	sem_init(&wshared.empty, 0,1);
	sem_init(&wshared.full, 0,0);
	
	pthread_create(&tid_producer, NULL, producer, NULL);
	pthread_create(&tid_consumer, NULL, consumer, NULL);

	pthread_join(tid_producer, NULL);
	pthread_join(tid_consumer, NULL);

	exit(0);
}


