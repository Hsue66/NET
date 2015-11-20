# include "header.h"

#define NUMPOLL 4

int main(int argc, char** argv)
{
	socklen_t len;
	struct sockaddr_in caddr, servaddr;
	struct pollfd pollfds[NUMPOLL];
	char buf[MAXLINE];
	int k, n, i;
	socklen_t clen = sizeof(caddr);
	
	int listenFD = socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	
 	if(bind(listenFD, (struct sockaddr *)&servaddr, sizeof(servaddr))==-1)     
	 {
		 printf("Can not bind.\n");
		 return -1;	
	 }

 
	if(listen(listenFD,BUFFER_SIZE) == -1)	
 	{
	
		 printf("Listen fail.\n");
		 return -1;		
 	}

	 printf("Waiting for clients..\n");


	/* set up poll fds */
	pollfds[0].fd = listenFD;
	pollfds[0].events = POLLIN;
	pollfds[0].revents = 0;
	
	for (i=1; i<NUMPOLL; i++) 
		pollfds[i].fd = -1;

//
//
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
			int connfd = accept(listenFD, (struct sockaddr*)&caddr, &clen);
//
//
			printf("connfd:%d\n", connfd);
			for(k=1; k<NUMPOLL; k++) 
			{
				if (pollfds[k].fd == -1) 
				{
					pollfds[k].fd = connfd;
					pollfds[k].events = POLLIN;
					pollfds[k].revents = 0;
					break;
				}
			}
			if (k == NUMPOLL) 
			{
				perror("too many clients");
				exit(0);
			}
		}

		/* read and write */
		for (i = 1; i < NUMPOLL; i++)
		{
			switch (/*pollfds[i].fd != -1 && */pollfds[i].revents)
			{
				case 0:
					/* no events */
					break;
				case POLLIN:
					/* echo incoming data */
					n = read(pollfds[i].fd, buf, MAXLINE);
					if(n==0) {
						close(pollfds[i].fd);
						pollfds[i].fd = -1;
					} else {
						writevn(pollfds[i].fd, buf, n);
						pollfds[i].revents = 0;
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
