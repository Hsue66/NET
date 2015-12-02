# include "header.h"
# include <time.h>

int name;
int main(int argc, char **argv)
{
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
		/** initialize buffer **/
		char Wbuffer[BUFFER_SIZE],Rbuffer[BUFFER_SIZE],Sub[BUFFER_SIZE];
		memset(&Wbuffer, 0, BUFFER_SIZE);
		memset(&Rbuffer, 0, BUFFER_SIZE);
		memset(&Sub, 0, BUFFER_SIZE);

		/** sed S tag **/
		sprintf(Sub, "%s","S");
		writevn(connFD, Sub, strlen(Sub));				
		
		/** send name **/
		srand(time(NULL));
		sprintf(Wbuffer,"%d",rand()%3);

		printf("%s\n",Wbuffer);
		fflush(stdout);

		name = atoi(Wbuffer);
		writevn(connFD, Wbuffer, strlen(Wbuffer));

		
		int n = readvn(connFD, Rbuffer, BUFFER_SIZE);
		Rbuffer[n]='\0';
		printf("%s\n",Rbuffer);
		fflush(stdout);
		
		/** registered well - Accepted  **/
		if(strcmp(Rbuffer,"! Accepted !")==0)
		{
			printf("hello\n");
			while(1);
																		 
		//	{
			/*	srand(time(NULL));
				sprintf(Wbuffer,"%d", rand()%3);
	
				name = atoi(Wbuffer);
				printf("%s\n",Wbuffer);
				fflush(stdout);

				writevn(connFD, Wbuffer, strlen(Wbuffer));
	
				int n = readvn(connFD, Rbuffer, BUFFER_SIZE);
				Rbuffer[n]='\0';
				printf("%s\n",Rbuffer);			
			
				fflush(stdout);

				printf("hello\n");
			while(1);
			*/
		//	}
		}
	//	else
	//	{
	//		printf("hello");
	//		while(1);
	//	}

	}
	fflush(stdout);
	return 0;
}
