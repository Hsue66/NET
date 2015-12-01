# include "header.h"

# define PUB 3
# define SUB 3

int topics[3]={0};

int main(int argc, char** argv)
{
//	int topics[3]={0};
	int	pub=0;
	int sub=0;
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
		int connfd = accept(listenFD, (struct sockaddr*)&caddr, &clen);
			
		char Wbuffer[BUFFER_SIZE], Rbuffer[BUFFER_SIZE];

		if(fork()==0 && pub<PUB)
		{
			printf("%d, %d, %d\n",topics[0],topics[1],topics[2]);
			int n = readvn(connfd, Rbuffer, BUFFER_SIZE);
			Rbuffer[n]='\0';
			
			if (topics[atoi(Rbuffer)]!=1) 
			{
				pub++;
				topics[atoi(Rbuffer)]=1;
				printf("%s\n",Rbuffer);
				sprintf(Wbuffer,"%s","! Accepted !");
				writevn(connfd,Wbuffer,strlen(Wbuffer));
			printf("%d, %d, %d\n",topics[0],topics[1],topics[2]);
			}
			else
			{
				sprintf(Wbuffer,"%s","! Denied !");
				writevn(connfd,Wbuffer,strlen(Wbuffer));
			}
		}
		else if(fork()==0 && pub == PUB) 
		{
			perror("too many publisher!");
		}
				
	}
	return 0;

}
