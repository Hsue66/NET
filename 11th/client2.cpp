# include "header.h"
# include <pthread.h>
/*
void *thread(void *vargp)
{
	char Rbuffer[BUFFER_SIZE];
	int connectFD = *((int *)vargp);
	pthread_detach(pthread_self());
	free(vargp);
	int r = readvn(connectFD, Rbuffer, BUFFER_SIZE);
	
	Rbuffer[r] = '\0';

	fputs(Rbuffer, stdout);

	printf("%d\n",r);

	fflush(stdout);

	return NULL;

}

*/
void childHandler(int signal)

{

		// when client die, child process die

		pid_t pid;

			while((pid = waitpid(-1, 0,WNOHANG)) > 0)

					{

								printf("Received signal %d from process %d\n",signal,pid);

									}

}
int main(int argc, char** argv)
{
	signal(SIGCHLD, childHandler);
		if(argc != 2)
		{
			printf("Usage: %s username! \n",argv[0]);
			return -1;
		}
		
		struct sockaddr_in connectSocket;
				
		memset(&connectSocket, 0, sizeof(connectSocket));		
		connectSocket.sin_family = AF_INET;		
		connectSocket.sin_port = htons(PORT);
		connectSocket.sin_addr.s_addr = inet_addr("127.0.0.1");

		int connectFD = socket(AF_INET, SOCK_STREAM, 0);
									
		if(connect(connectFD, (struct sockaddr*) &connectSocket, sizeof(connectSocket)) == -1)
		{
			printf("Cannot connect\n");
			return -1;
		}
			
		else
		{
			char Rbuffer[BUFFER_SIZE];
			char Wbuffer[BUFFER_SIZE];
				//sprintf(Wbuffer, "%s", argv[1]);
				//writevn(connectFD, Wbuffer, strlen(Wbuffer));
			
/*				printf("Message: ");
				scanf("%s", Wbuffer);	
				writevn(connectFD, Wbuffer, strlen(Wbuffer));
				int r = readvn(connectFD, Rbuffer, BUFFER_SIZE);
				Rbuffer[r] = '\0';
				fputs(Rbuffer, stdout);
				printf("%d\n",r);
				fflush(stdout);
				
				int *addr = (int*)(malloc(sizeof(int)));
				*addr = 1;
				pthread_create(&tid,NULL,thread,addr);
i*/
			while(1)
			{
			if(fork()==0)
			{
				while(1)
				{
					int r = readvn(connectFD, Rbuffer, BUFFER_SIZE);
					Rbuffer[r] = '\0';
					printf("%s\n",Rbuffer);
					fflush(stdout);
					close(connectFD);
					exit(0);
				}
			}
			else
			{
	//			while(1)
	//			{
//					 sprintf(Wbuffer, "%s", argv[1]);


//					 writevn(connectFD, Wbuffer, strlen(Wbuffer));
//					fflush(stdin);
					
					 printf("Message: ");
					char str[BUFFER_SIZE];
					 scanf("%s", str);
					 sprintf(Wbuffer,"%s : %s",argv[1],str);
					 writevn(connectFD, Wbuffer, strlen(Wbuffer));

					fflush(stdin);					
	//			}
			
			}																									
			}
		}
		close(connectFD);		
		return 0;
}
