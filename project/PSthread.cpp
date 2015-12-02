# include "header.h"
# include <pthread.h>

# define PUB 3
# define SUB 3

struct MSGA msgA;
struct MSGB msgB;
struct MSGC msgC;

int Rtopics[3]={0}; // write topic variable
int Stopics[3]={3,3,3};	// read topic variable
int updates[3][3];

int pub =0;
int sub =0;

void* clientFunc(void* arguments)
{
	int connfd = *((int*)arguments);
	pthread_detach(pthread_self());
	free(arguments);

	char Wbuffer[BUFFER_SIZE], Rbuffer[BUFFER_SIZE];
		
	int n = readvn(connfd, Rbuffer, BUFFER_SIZE);
	Rbuffer[n]='\0';
	printf("%s\n",Rbuffer);

	
	/** when publisher trying to connect **/
	if(strcmp(Rbuffer,"P")==0)
	{
		if(pub>=PUB)	
		{	// publisher is FULL
			printf("! too many PUBLISHER !\n");
			close(connfd);
		}
		else
		{	// not FULL
			int choice=0;		
			do		// try to regist TOPIC
			{
//				printf("%d : %d, %d, %d\n",pub,Rtopics[0],Rtopics[1],Rtopics[2]);
				int n = readvn(connfd, Rbuffer, BUFFER_SIZE);
				Rbuffer[n]='\0';
	
				if (Rtopics[atoi(Rbuffer)]==0) 
				{	// can regist TOPIC 
					pub++;
					Rtopics[atoi(Rbuffer)]=1;			// register topic
					printf("%s\n",Rbuffer);
					sprintf(Wbuffer,"%s","! Accepted !");	
					writevn(connfd,Wbuffer,strlen(Wbuffer));
//					printf("%d, %d, %d\n",Rtopics[0],Rtopics[1],Rtopics[2]);
					choice=0;

					while(1)
					{
						printf("hello\n");
						/** receive msg from publisher **/
						if((n = readvn(connfd, Rbuffer, BUFFER_SIZE))>0)
						{	// read client name first
							Rbuffer[n]='\0';
							int name = atoi(Rbuffer);

							if(name == 0)
							{	
								if((n = readvn(connfd, Rbuffer, BUFFER_SIZE))>0)
								{
									memcpy(&msgA, Rbuffer,sizeof(msgA));
									printf("%c : %s || %s\n",msgA.id,msgA.ext,msgA.msg);
									
									// alarm for updates
									for(int i=0; i<3; i++)
									{
										updates[i][name]=1;
									}
//									printf("SET : %d %d %d\n",updates[0][0],updates[1][0],updates[2][0]);
								}
								else
								{
									msgA.id='N';
									printf("%c\n",msgA.id);
								}
							}
							else if(name == 1)
							{
								if((n = readvn(connfd, Rbuffer, BUFFER_SIZE))>0)
								{
									memcpy(&msgB, Rbuffer,sizeof(msgA));
									printf("%c : %s\n",msgB.id,msgB.msg);
									
									// alarm for updates
									for(int i=0; i<3; i++)
										updates[i][name]=1;
								}
								else
								{
									msgB.id='N';
								}
							}	
							else
							{
								if((n = readvn(connfd, Rbuffer, BUFFER_SIZE))>0)
								{
									memcpy(&msgC, Rbuffer,sizeof(msgC));
									printf("%c : %d || %s\n",msgC.id,msgC.num,msgC.msg);
									// alarm for updates
									for(int i=0; i<3; i++)
										updates[i][name]=1;
								}
								else
								{
									msgC.id='N';
								}
							}
												
							sprintf(Wbuffer,"%s","! Received !");	
							writevn(connfd,Wbuffer,strlen(Wbuffer));
						}		
					}

				}
				else
				{	// already registed TOPIC
					sprintf(Wbuffer,"%s","! Denied !");
					writevn(connfd,Wbuffer,strlen(Wbuffer));
					choice=1;
				}		
			}while(choice!=0);	// loop if alredy registered
			
//			printf("%d : %d, %d, %d\n",pub,Rtopics[0],Rtopics[1],Rtopics[2]);
			fflush(stdout);
		}
		close(connfd);
	}

	/** when subscriber trying to connect **/
	else if(strcmp(Rbuffer,"S")==0)
	{
		if(sub>=SUB)	
		{
			printf("too many SUBSCRIBER!\n");
			close(connfd);
		}
		else
		{
//			printf("%d : %d, %d, %d\n",sub, Stopics[0],Stopics[1],Stopics[2]);
            int n = readvn(connfd, Rbuffer, BUFFER_SIZE);
								
			if(Stopics[sub]==3 && n>0)	
			{	// if subscriber doesn't register topic to read
				Rbuffer[n]='\0';
				printf("%s\n",Rbuffer);	
				fflush(stdout);
				int state = sub;
				Stopics[state] = atoi(Rbuffer);
				sprintf(Wbuffer,"%s","! Accepted !");
				writevn(connfd,Wbuffer,strlen(Wbuffer));
//				printf("%d : %d, %d, %d\n",sub,Stopics[0],Stopics[1],Stopics[2]);
				sub++;
				
				while(1)
				{
					if(Stopics[state]==0)
					{
						if(msgA.id!='N' && updates[state][Stopics[state]]==1)
						{

									printf("SET %c : %s || %s\n",msgA.id,msgA.ext,msgA.msg);
							writevn(connfd,(char *)&msgA, sizeof(msgA));
							updates[state][Stopics[state]]=0;
						}
					}
					else if(Stopics[state]==1)
					{
						if(msgB.id!='N' && updates[state][Stopics[state]]==1)
						{
									printf("SET %c : %s\n",msgB.id,msgB.msg);
							writevn(connfd,(char *)&msgB, sizeof(msgB));
							updates[state][Stopics[state]]=0;
						}
					}
					else
					{
						if(msgC.id!='N' && updates[state][Stopics[state]]==1)
						{
									printf("SET %c : %d || %s\n",msgC.id,msgC.num,msgC.msg);
							writevn(connfd,(char *)&msgC, sizeof(msgC));
							updates[state][Stopics[state]]=0;
						}
					}
				}
			}
				/*if(state == 0)
				{
					if(msgA.id!='N' && updates[state][Stopics[state]]==1)
					{
//						if(Stopics[state]==0)
//							printf("%d %d %d\n",updates[0][0],updates[1][0],updates[2][0]);
						

						writevn(connfd,(char *)&msgA, sizeof(msgA));
						updates[state][Stopics[state]]=0;
	
						if(Stopics[state]==0)
						{

									printf("%c : %s || %s\n",msgA.id,msgA.ext,msgA.msg);
							printf("0 : %d %d %d\n",updates[0][0],updates[1][0],updates[2][0]);
						}
					}
				}
				else if(state == 1)
				{
					if(msgB.id!='N' && updates[state][Stopics[state]]==1)
					{
						writevn(connfd,(char *)&msgB, sizeof(msgB));
						updates[state][Stopics[state]]=0;
							if(Stopics[state]==0)
							{

									printf("%c : %s\n",msgB.id,msgB.msg);
								printf("1 : %d %d %d\n",updates[0][0],updates[1][0],updates[2][0]);
							}
					}
				}
				else
				{
					if(msgC.id!='N' && updates[state][Stopics[state]]==1)
					{
						writevn(connfd,(char *)&msgC, sizeof(msgC));
						updates[state][Stopics[state]]=0;
								if(Stopics[state]==0)
								{
									printf("%c : %d || %s\n",msgC.id,msgC.num,msgC.msg);
							printf("2 : %d %d %d\n",updates[0][0],updates[1][0],updates[2][0]);								}
					}
				}
			
			}
*//*				while(1)
				{
					
				}
				*/
			else
			{
				close(connfd);
			}
		}
	}
	
}

int main(int argc, char** argv)
{
	/** initialize msg **/
	msgA = initA(msgA);
	msgB = initB(msgB);
	msgC = initC(msgC);
	
	/** init updates array **/
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
			updates[i][j]=0;
	}

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
