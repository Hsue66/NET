# include "header.h"
# include <time.h>

int main(int argc, char **argv)
{
	char retry[BUFFER_SIZE];
	sprintf(retry,"%s","! Denied !");

	if(argc != 2)
	{
		printf("Usage: %s username!\n",argv[0]);
		return -1;
	}

	struct sockaddr_in connaddr;
	memset(&connaddr, 0, sizeof(connaddr));
	connaddr.sin_family = AF_INET;
	connaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	connaddr.sin_port = htons(PORT);

	int connFD = socket(AF_INET, SOCK_STREAM, 0);

	if(connect(connFD, (struct sockaddr*) &connaddr, sizeof(connaddr)) == -1)
	{
		printf("Cannot connect.\n");
		return -1;
	}
	else
	{
		char Wbuffer[BUFFER_SIZE],Rbuffer[BUFFER_SIZE],Pub[BUFFER_SIZE];
		sprintf(Pub, "%s","S");
		writevn(connFD, Pub, strlen(Pub));
		
		int temp[3]={0};	
		while(1)
		{
			srand(time(NULL));
			do
			{
			sprintf(Wbuffer,"%d", rand()%3);
			}while(temp[atoi(Wbuffer)]==1);

			printf("%s\n",Wbuffer);
			fflush(stdout);

			writevn(connFD, Wbuffer, strlen(Wbuffer));
	
			int n = readvn(connFD, Rbuffer, BUFFER_SIZE);
			Rbuffer[n]='\0';
			printf("%s\n",Rbuffer);			
			printf("%s\n",retry);			
			
			printf("%d\n",strcmp(Rbuffer,retry));
			fflush(stdout);

				if(strcmp(Rbuffer,retry)!=0)
				{
					printf("hello\n");
					while(1);
				}
				else
				{
					sleep(1);
					temp[atoi(Wbuffer)]=1;
					printf("%d,%d,%d\n",temp[0],temp[1],temp[2]);
					continue;
				}
		}
	}
}
