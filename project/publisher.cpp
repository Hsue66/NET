# include "header.h"
# include <time.h>

/** make timestamp **/
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
		
		memset(&Wbuffer, 0, BUFFER_SIZE);
		memset(&Rbuffer, 0, BUFFER_SIZE);
		memset(&Pub, 0, BUFFER_SIZE);

		sprintf(Pub, "%s","P");
		writevn(connFD, Pub, strlen(Pub));
		
		int temp[3]={0};	
		while(1)
		{
			do
			{
				srand(time(NULL));
				sprintf(Wbuffer,"%d", rand()%3);
			}while(temp[atoi(Wbuffer)]==1);

			name=atoi(Wbuffer);
			printf("%d\n",name);
			fflush(stdout);

			writevn(connFD, Wbuffer, strlen(Wbuffer));
	
			int n = readvn(connFD, Rbuffer, BUFFER_SIZE);
			Rbuffer[n]='\0';
			printf("%s\n",Rbuffer);			
			
			fflush(stdout);

			/** registered well - Accepted  **/
			if(strcmp(Rbuffer,"! Denied !")!=0)
			{
				while(1){

				sprintf(Wbuffer,"%d",name);
				writevn(connFD, Wbuffer, strlen(Wbuffer));
			
				printf("MYNAME %d\n",name);
	
				/** set now time **/
				now = time(NULL);
				t = localtime(&now);
					
				if(name == 0)
				{
					sprintf(Wbuffer,"%s","A");
					writevn(connFD, Wbuffer, strlen(Wbuffer));
					
					sprintf(Wbuffer,"NEW MSG : %d AT %s",rand()%1000,timestamp(t));
					writevn(connFD, Wbuffer, strlen(Wbuffer));

				}
				else if(name == 1)
				{
					sprintf(Wbuffer,"%s","B");
					writevn(connFD, Wbuffer, strlen(Wbuffer));
					
					sprintf(Wbuffer,"NEW MSG : %d AT %s",rand()%100,timestamp(t));
					writevn(connFD, Wbuffer, strlen(Wbuffer));
				}
				else 
				{
					sprintf(Wbuffer,"%s","C");
					writevn(connFD, Wbuffer, strlen(Wbuffer));
					
					sprintf(Wbuffer,"NEW MSG : %d AT %s",rand()%10,timestamp(t));
					writevn(connFD, Wbuffer, strlen(Wbuffer));
				}
				if( (n = readvn(connFD,Rbuffer,BUFFER_SIZE))>0)
				{
					Rbuffer[n]='\0';
					printf("%s\n",Rbuffer);
				}
//				while(1);				
				sleep(5);
				}
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
	fflush(stdout);
	return 0;
}
