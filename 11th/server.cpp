# include "header.h"
# define FDSETSIZE 4

int main(int argc, char** argv)
{
	// make socket structure and socket
	struct sockaddr_in cliaddr, servaddr,peerSocket;
	
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	int listenFD = socket(AF_INET, SOCK_STREAM, 0);

	// check bind or not
	if(bind(listenFD, (struct sockaddr *)&servaddr, sizeof(servaddr))==-1)		
	{
		printf("Can not bind.\n");
		return -1;
	}							

	// check listen or not
	if(listen(listenFD,BUFFER_SIZE) == -1)
	{
		printf("Listen fail.\n");
		return -1;
	}

	printf("Waiting for clients..\n");

	// variables
	int num=0;				// number of connect
	int maxfd,connfd;
	char cname[FDSETSIZE][BUFFER_SIZE];	// name array
	int client[FDSETSIZE];				// client array

	// init client
	for(int i=0; i<FDSETSIZE; i++)
	{
		client[i]=-1;
	}

	fd_set set;
	socklen_t clilen;

	maxfd = listenFD+1;
	FD_ZERO(&set);
	
	char Rbuffer[BUFFER_SIZE],Wbuffer[BUFFER_SIZE],Sbuffer[BUFFER_SIZE];

	// init buffer
	memset(&Sbuffer,0,sizeof(Sbuffer));
	memset(&Rbuffer,0,sizeof(Rbuffer));
	memset(&Wbuffer,0,sizeof(Wbuffer));

	// deal client access
	for(; ;)
	{
		FD_SET(listenFD, &set);

		if((num-1) >= 0 )
			maxfd = client[num-1]+1;

		// if client in -> set client[]
		for(int i=0; i<FDSETSIZE; i++)
		{
			if(client[i]!= -1)
				FD_SET(client[i],&set);
		}

		// do select
		if( select(maxfd, &set, NULL,NULL,NULL)<0)
		{
			printf("select error\n");
			return -1;
		}

		// if set
		if(FD_ISSET(listenFD,&set))
		{
			// make listenfd to active socket
			clilen = sizeof(cliaddr);
			connfd = accept(listenFD, (struct sockaddr *) &cliaddr, &clilen);

			// connfd exist
			if(connfd!= -1)
			{
				if(num<4)
				{
					// set client[] to connfd
					client[num] = connfd;
					// read username from client and save it to cname 
					readvn(connfd,Rbuffer,BUFFER_SIZE);
					printf("User : %s\n",Rbuffer);
					sprintf(cname[num],"%s",Rbuffer);

					// send msg to client
					sprintf(Wbuffer,"%s","! Welcome to Server !");
					writevn(connfd,Wbuffer,strlen(Wbuffer));	

					// send msg to every client for alarm new client
					sprintf(Sbuffer,"! NEW : %s !",cname[num]);
					for(int i=0; i<FDSETSIZE; i++)
					{
						if(i != num)
							writevn(client[i],Sbuffer,strlen(Sbuffer));	
					}
					num++;
				}
				else	// over clients
				{
					// send msg to client can't come int
					sprintf(Wbuffer,"%s","! Sorry, Too many people in here !");
					writevn(connfd,Wbuffer,strlen(Wbuffer));
					num--;
				}
			}

		}

		// read & write
		for(int i=0; i<FDSETSIZE; i++)
		{
			// only for exist clients
			if(FD_ISSET(client[i], &set)&&client[i]!=-1)
			{
				// read msg from client
				int n=readvn(client[i],Rbuffer,BUFFER_SIZE);
				if(n>0)
				{
					Rbuffer[n]= '\0';
					
					// send it to all clients
					for(int j =0; j<FDSETSIZE; j++)
					{
						if(i!=j)
						{
							// formal msg
							if(strcmp(Rbuffer,"BYE")!=0)
							{
								sprintf(Wbuffer,"%s : %s",cname[i],Rbuffer);
								writevn(client[j],Wbuffer,strlen(Wbuffer));
							}
							// bye msg
							else
							{
								sprintf(Wbuffer,"!%s : %s!",Rbuffer,cname[i]);	
								writevn(client[j],Wbuffer,strlen(Wbuffer));
							}
						}
						// delete bye client
						else
						{
							if(strcmp(Rbuffer,"BYE")==0)
								client[i] = -1;
						}
					}
					if(strcmp(Rbuffer,"BYE")==0)
						num--;	
				}
			}
		}
		
	}
}
