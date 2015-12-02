# include "header.h"
# include <pthread.h>

# define PUB 3
# define SUB 3

struct MSGA msgA;
struct MSGB msgB;
struct MSGC msgC;

int Rtopics[3]={0}; // register topic variable
int pub =0;
int sub =0;
int full=0;

void* clientFunc(void* arguments)
{
	int connfd = *((int*)arguments);
	pthread_detach(pthread_self());
	free(arguments);

	if(pub>=PUB)	
		close(connfd);
	
	else
	{
//		msgA = initA(msgA);
//		msgB = initB(msgB);
//		msgC = initC(msgC);


		char Wbuffer[BUFFER_SIZE], Rbuffer[BUFFER_SIZE];
		
		int n = readvn(connfd, Rbuffer, BUFFER_SIZE);
		Rbuffer[n]='\0';
		printf("%s\n",Rbuffer);

		if(strcmp(Rbuffer,"P")==0)
		{

			int choice=0;		
			do
			{
				printf("%d : %d, %d, %d\n",pub,Rtopics[0],Rtopics[1],Rtopics[2]);
				int n = readvn(connfd, Rbuffer, BUFFER_SIZE);
				Rbuffer[n]='\0';
	
				if (Rtopics[atoi(Rbuffer)]==0) 
				{
					pub++;
					Rtopics[atoi(Rbuffer)]=1;
					printf("%s\n",Rbuffer);
					sprintf(Wbuffer,"%s","! Accepted !");	
					writevn(connfd,Wbuffer,strlen(Wbuffer));
					printf("%d, %d, %d\n",Rtopics[0],Rtopics[1],Rtopics[2]);
					choice=0;



		while(1){

					/** receive msg from publisher **/
					if((n = readvn(connfd, Rbuffer, BUFFER_SIZE))>0)
					{	// read client name first
						Rbuffer[n]='\0';
						int name = atoi(Rbuffer);
//						printf("R NAME %d\n",name);					

						if(name == 0)
						{
							if((n = readvn(connfd, Rbuffer, BUFFER_SIZE))>0)
							{	// read msg
								Rbuffer[n]='\0';
								sprintf(msgA.id,"%s", Rbuffer);
								if((n = readvn(connfd, Rbuffer, BUFFER_SIZE))>0)
								{
									Rbuffer[n]='\0';
									sprintf(msgA.msg,"%s", Rbuffer);
									
									printf("%s: %s\n",msgA.id,msgA.msg);
								}
								else
									sprintf(msgA.id,"%s","N");
							}
							else
								sprintf(msgA.id,"%s","N");
						}
						else if(name == 1)
						{
							if((n = readvn(connfd, Rbuffer, BUFFER_SIZE))>0)
							{	// read msg
								Rbuffer[n]='\0';
								sprintf(msgB.id,"%s", Rbuffer);
								if((n = readvn(connfd, Rbuffer, BUFFER_SIZE))>0)
								{
									Rbuffer[n]='\0';
									sprintf(msgB.msg,"%s", Rbuffer);
									
									printf("%s: %s\n",msgB.id,msgB.msg);
								}
								else
									sprintf(msgB.id,"%s","N");
							}
							else
								sprintf(msgB.id,"%s","N");
						}
						else
						{
							if((n = readvn(connfd, Rbuffer, BUFFER_SIZE))>0)
							{	// read msg
								Rbuffer[n]='\0';
								sprintf(msgC.id,"%s",Rbuffer);
								if((n = readvn(connfd, Rbuffer, BUFFER_SIZE))>0)
								{
									Rbuffer[n]='\0';
									sprintf(msgC.msg,"%s",Rbuffer);
									
									printf("%s: %s\n",msgC.id,msgC.msg);
								}
								else
									sprintf(msgC.id,"%s","N");
							}
							else
								sprintf(msgC.id,"%s","N");
						}

												
						sprintf(Wbuffer,"%s","! Received !");	
						writevn(connfd,Wbuffer,strlen(Wbuffer));
					}		
			}

				}
				else
				{
					sprintf(Wbuffer,"%s","! Denied !");
					writevn(connfd,Wbuffer,strlen(Wbuffer));
					choice=1;
				}		
			}while(choice!=0);
				printf("%d : %d, %d, %d\n",pub,Rtopics[0],Rtopics[1],Rtopics[2]);
			fflush(stdout);
		}
		else
		{

		}
		close(connfd);
	}
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

		if(pub<PUB)
		{
			int *connfd = (int *)malloc(sizeof(int));
			*connfd = accept(listenFD, (struct sockaddr*)&caddr, &clen);
			
			pthread_create(&tid, NULL, clientFunc, connfd);
		}
		else
		{
			if(full==0)
			{
				printf("too many publisher!\n");
				fflush(stdout);	
				int *connFD = (int *)malloc(sizeof(int));
				*connFD = accept(listenFD, (struct sockaddr*)&caddr, &clen);
			
				pthread_create(&tid, NULL, clientFunc, connFD);
				full=1;
			}
		}
	}
	return 0;

}
