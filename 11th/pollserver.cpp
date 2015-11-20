# include "header.h"

#define NUMPOLL 5

int main(int argc, char** argv)
{
	// variables
	socklen_t len;
	struct sockaddr_in caddr, servaddr;
	struct pollfd pollfds[NUMPOLL];
	char buf[MAXLINE];
	int k, n, i;
	socklen_t clen = sizeof(caddr);
	
	// make socket structure and socket
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	
	// check bind or not
 	if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr))==-1)     
	 {
		 printf("can not bind.\n");
		 return -1;	
	 }


 	// check listen or not
	if(listen(listenfd,BUFFER_SIZE) == -1)	
 	{
		 printf("listen fail.\n");
		 return -1;		
 	}

	 printf("waiting for clients..\n");


	/* set up poll fds */
	pollfds[0].fd = listenfd;
	pollfds[0].events = POLLIN;
	pollfds[0].revents = 0;
	
	// init poll fds
	for (i=1; i<NUMPOLL; i++) 
		pollfds[i].fd = -1;

	char cname[NUMPOLL][BUFFER_SIZE];
	char Rbuffer[BUFFER_SIZE], Wbuffer[BUFFER_SIZE];


	while (1) 
	{
		/* poll and use -1 timeout (infinite) */
		if (poll(pollfds, NUMPOLL, -1) < 0) 
		{
			perror("poll failed");
			exit(3);
		}
		/* accept a new client connection */
		if (pollfds[0].revents == POLLIN) 
		{
			int connfd = accept(listenfd, (struct sockaddr*)&caddr, &clen);

			// read username
			int n = readvn(connfd, Rbuffer, BUFFER_SIZE);
			Rbuffer[n]='\0';

			int cnt=1;
			// allow client to came in chat server
			for(k=1; k<NUMPOLL; k++) 
			{
				if (pollfds[k].fd == -1) 
				{
					pollfds[k].fd = connfd;
					pollfds[k].events = POLLIN;
					pollfds[k].revents = 0;
					printf("User: %s\n",Rbuffer);
					// send msg to client
				 	sprintf(Wbuffer,"%s","! welcome to server !");
				 	writevn(connfd,Wbuffer,BUFFER_SIZE);
					// save client name
				 	sprintf(cname[k],"%s",Rbuffer);
				 	cnt = k;
					break;
				}
			}
			// not allowed to came in
			if (k == NUMPOLL) 
			{
				// send can't msg to client
				sprintf(Wbuffer,"%s","! Sorry, Too many people in here!");
				writevn(connfd,Wbuffer,BUFFER_SIZE);
				perror("too many clients");
			}
			// alarm to exist clients
			else
			{
				for(k=1; k<NUMPOLL; k++)
				{
					if(k!=cnt)
					{
						sprintf(Wbuffer,"! NEW : %s !",cname[cnt]);
						writevn(pollfds[k].fd,Wbuffer,strlen(Wbuffer));
					}
				}
			}
		}

		/* read and write */
		for (i = 1; i < NUMPOLL; i++)
		{
			switch (pollfds[i].revents)
			{
				case 0:
					/* no events */
					break;
				case POLLIN:
					// read msg from client
					n = readvn(pollfds[i].fd, buf, MAXLINE);
					buf[n]='\0';
						
					// send to other clients
					for(int j=1; j<NUMPOLL; j++)
					{
						if(pollfds[j].fd != -1 && j!=i)
						{	
							// not BYE msg
							if(strcmp(buf,"BYE")!=0)
							{
								sprintf(Wbuffer,"%s : %s",cname[i],buf);
								writevn(pollfds[j].fd,Wbuffer,strlen(Wbuffer));
								pollfds[j].revents=0;
							}
							// BYE msg
							else
							{
								sprintf(Wbuffer,"! %s : %s !",buf,cname[i]);
								writevn(pollfds[j].fd,Wbuffer,strlen(Wbuffer));
								pollfds[i].fd = -1;	
							}
						}
					}
					break;
				default:
					perror("error event");
					close(pollfds[i].fd);
					pollfds[i].fd = -1;
					pollfds[i].revents = 0;
					//exit(4);
			}
		}
	}
}
