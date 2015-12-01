# include "header.h"

# define PUB 4
# define SUB 4

int main(int argc, char** argv)
{
	int topics[3]={0};

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

	struct pollfd pubfds[PUB], subfds[SUB];

	pubfds[0].fd = listenFD;
	pubfds[0].events = POLLIN;
	pubfds[0].revents = 0;

	subfds[0].fd = listenFD;
	subfds[0].events = POLLIN;
	subfds[0].revents = 0;

	for(int i=1; i<PUB; i++)
	{
		pubfds[i].fd = -1;
		subfds[i].fd = -1;
	}


	while(1)
	{
		/* poll and use -1 timeout (infinite) */
		
		if (poll(pubfds, PUB, -1) < 0) 
		{
			perror("poll failed");
			exit(3);
		}
		
		/* accept a new client connection */
		
		if (pubfds[0].revents == POLLIN) 
		{
			struct sockaddr_in caddr;
			socklen_t clen= sizeof(caddr);
			int connfd = accept(listenFD, (struct sockaddr*)&caddr, &clen);
			
			char Wbuffer[BUFFER_SIZE], Rbuffer[BUFFER_SIZE];

			//printf("publisher: %d\n", connfd);
			int n = readvn(connfd, Rbuffer, BUFFER_SIZE);
			Rbuffer[n]='\0';
//			topics[atoi(Rbuffer)]=1;
			//printf("%d\n",atoi(Rbuffer));
			
			int k;
			for(k=1; k<PUB; k++) 
			{
				if (pubfds[k].fd == -1 && topics[atoi(Rbuffer)]!=1) 
				{
				//	if(topics[atoi(Rbuffer)]==1)
				//	{
				//		sprintf(Wbuffer,"%s","! Denied !");
				//		writevn(connfd,Wbuffer,strlen(Wbuffer));
				//	}
				//	else
				//	{
						pubfds[k].fd = connfd;
						pubfds[k].events = POLLIN;
						pubfds[k].revents = 0;
						topics[atoi(Rbuffer)]=1;
						printf("%s\n",Rbuffer);
						sprintf(Wbuffer,"%s","! Accepted !");
						writevn(connfd,Wbuffer,strlen(Wbuffer));
						break;
				//	}
				}
				else if(pubfds[k].fd==-1 && topics[atoi(Rbuffer)]==1)
				{
						sprintf(Wbuffer,"%s","! Denied !");
						writevn(connfd,Wbuffer,strlen(Wbuffer));
						break;
				}
			}
				
			if (k == PUB) 
			{
				perror("too many publisher!");
			}
				
		}
	}

}
