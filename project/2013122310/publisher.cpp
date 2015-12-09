# include "header.h"
# include <time.h>

/** 주3.make timestamp **/
char* timestamp(struct tm *t)
{
	static char temp[BUFFER_SIZE];
	sprintf(temp,"%04d-%02d-%-2d %02d:%02d:%02d",t->tm_year +1900, t->tm_mon+1,
			t->tm_mday,t->tm_hour, t->tm_min, t->tm_sec);
	return temp;
}

int main(int argc, char **argv)
{
	int name;
	/** check username input **/
	if(argc != 2)
	{
		printf("Usage: %s username!\n",argv[0]);
		return -1;
	}

	/** set time variable**/
	time_t now;
	struct tm *t;

	/** 주1.connect with broker  **/
	struct sockaddr_in connaddr;
	memset(&connaddr, 0, sizeof(connaddr));
	connaddr.sin_family = AF_INET;
	connaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	connaddr.sin_port = htons(PORT);

	int connFD = socket(AF_INET, SOCK_STREAM, 0);

	if(connect(connFD, (struct sockaddr*) &connaddr, sizeof(connaddr)) == -1)
	{	// connection fail
		printf("Cannot connect.\n");
		return -1;
	}
	else
	{	// connected well
		char Wbuffer[BUFFER_SIZE],Rbuffer[BUFFER_SIZE],Pub[BUFFER_SIZE];
		memset(&Wbuffer, 0, BUFFER_SIZE);
		memset(&Rbuffer, 0, BUFFER_SIZE);
		memset(&Pub, 0, BUFFER_SIZE);

		sprintf(Pub, "%s","P");
		writevn(connFD, Pub, strlen(Pub));
		
		int temp[3]={0};	
		while(1)
		{
			/** 주2-2.select not overlapped topic**/
			do
			{
				srand(time(NULL));
				sprintf(Wbuffer,"%d", rand()%3);
			}while(temp[atoi(Wbuffer)]==1);

			name=atoi(Wbuffer);
			printf("%d\n",name);
			fflush(stdout);

			/** 주2-1.register topic at broker **/
			writevn(connFD, Wbuffer, strlen(Wbuffer));
	
			int n = readvn(connFD, Rbuffer, BUFFER_SIZE);
			Rbuffer[n]='\0';
			printf("%s\n",Rbuffer);			
			
			fflush(stdout);

			/** registered well - Accepted  **/
			if(strcmp(Rbuffer,"! Denied !")!=0)
			{
				/** 주3.send message to broker **/
				while(1)
				{
					sprintf(Wbuffer,"%d",name);
					writevn(connFD, Wbuffer, strlen(Wbuffer));
			
					printf("PUBLISH %d\n",name);
		
					/** set now time **/
					now = time(NULL);
					t = localtime(&now);
					
					if(name == 0)
					{
						struct MSGA msgA;
						msgA.id ='A';
						sprintf(msgA.ext,"I'm topic %d",name);
						sprintf(msgA.msg,"Hello | AT %s",timestamp(t));
						writevn(connFD, (char *)&msgA, sizeof(msgA));
					}
					else if(name == 1)
					{	
						struct MSGB msgB;
						msgB.id ='B';
						sprintf(msgB.msg,"Thankyou | AT %s",timestamp(t));
						writevn(connFD, (char *)&msgB, sizeof(msgB));
					}
					else 
					{
						struct MSGC msgC;
						msgC.id='C';
						msgC.num = name;
						sprintf(msgC.msg,"I can send INT | AT %s",timestamp(t));
						writevn(connFD, (char *)&msgC, sizeof(msgC));
					}

					/** receive sended well message **/
					if( (n = readvn(connFD,Rbuffer,BUFFER_SIZE))>0)
					{
						Rbuffer[n]='\0';
						printf("%s\n",Rbuffer);
					}
					sleep(1);		// sleep for 1 second
				}
			}
			/** 주2-2.if selection is overlapped then try again **/
			else
			{
				sleep(1);	
				temp[atoi(Wbuffer)]=1;
				printf("%d,%d,%d\n",temp[0],temp[1],temp[2]);
				continue;
			}
		}
	}
	fflush(stdout);
	return 0;
}
