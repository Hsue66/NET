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

	/** 주1. connect with broker **/
	struct sockaddr_in connaddr;
	memset(&connaddr, 0, sizeof(connaddr));
	connaddr.sin_family = AF_INET;
	connaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	connaddr.sin_port = htons(PORT);

	int connFD = socket(AF_INET, SOCK_STREAM, 0);
	
	/** Trying to connect **/
	if(connect(connFD, (struct sockaddr*) &connaddr, sizeof(connaddr)) == -1)
	{
		printf("Cannot connect.\n");
		return -1;
	}
	else
	{	// connection established

		/** initialize buffer **/
		char Wbuffer[BUFFER_SIZE],Rbuffer[BUFFER_SIZE],Sub[BUFFER_SIZE];
		memset(&Wbuffer, 0, BUFFER_SIZE);
		memset(&Rbuffer, 0, BUFFER_SIZE);
		memset(&Sub, 0, BUFFER_SIZE);

		/** send S tag **/
		sprintf(Sub, "%s","S");
		writevn(connFD, Sub, strlen(Sub));				
		
		/** 주2-3. send topic name - randomly **/
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
			printf("Hello\n");
			while(1)
			{
				n= readvn(connFD, Rbuffer, BUFFER_SIZE);
				/** 주4.read topic from broker **/
				if(n>0)
				{
					if(name==0)
					{
						struct MSGA msgA;
						memcpy(&msgA,Rbuffer, sizeof(msgA));
						printf("%c : %s || %s\n",msgA.id, msgA.ext, msgA.msg);
					}
					else if(name ==1)
					{
						struct MSGB msgB;
						memcpy(&msgB,Rbuffer, sizeof(msgB));
						printf("%c : %s\n",msgB.id, msgB.msg);
					}
					else
					{
						struct MSGC msgC;
						memcpy(&msgC,Rbuffer, sizeof(msgC));
						printf("%c : %d || %s\n",msgC.id, msgC.num, msgC.msg);
					}
				}

			}
																		 
		}
	}
	fflush(stdout);
	return 0;
}
