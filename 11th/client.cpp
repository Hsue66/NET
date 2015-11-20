# include "header.h"


int main(int argc, char** argv)
{
	// define die condition
	char die[BUFFER_SIZE];
	sprintf(die,"%s","! Sorry, Too many people in here!");
	pid_t pid;

	// check username exist or not
	if(argc != 2)
	{
		printf("Usage: %s username! \n",argv[0]);
		return -1;
	}

	// make socket struct and connect socket 
	struct sockaddr_in connectSocket;				
	memset(&connectSocket, 0, sizeof(connectSocket));		
	connectSocket.sin_family = AF_INET;		
	connectSocket.sin_port = htons(PORT);
	connectSocket.sin_addr.s_addr = inet_addr("127.0.0.1");

	int	connectFD = socket(AF_INET, SOCK_STREAM, 0);

	// TRY connect 
	if(connect(connectFD, (struct sockaddr*) &connectSocket, sizeof(connectSocket)) == -1)
	{
		printf("Cannot connect\n");
		return -1;
	}
	
	// connected well
	else
	{
		char Rbuffer[BUFFER_SIZE],Wbuffer[BUFFER_SIZE],Sbuffer[BUFFER_SIZE];
		
		// send username
		sprintf(Wbuffer, "%s",argv[1]);
		writevn(connectFD, Wbuffer,strlen(Wbuffer));
		// receive INFO about connected or not
		int n =readvn(connectFD, Rbuffer, BUFFER_SIZE);
		Rbuffer[n] = '\0';
		printf("%s\n",Rbuffer);

		// possible to chat or not //
		if(strcmp(Rbuffer,die)!=0)	// possible to chat
		{
			// make child to read msg
			if((pid=fork())==0)
			{
				while(1)
				{
					int n =readvn(connectFD, Rbuffer, BUFFER_SIZE);
					Rbuffer[n] = '\0';
					printf("%s\n",Rbuffer);
				}
			}
			// parent to send msg
			else
			{
				while(1)
				{
					scanf("%s",Sbuffer);
					sprintf(Wbuffer,"%s",Sbuffer);
					writevn(connectFD,Wbuffer,strlen(Wbuffer));

					// send BYE to finish chat
					if(strcmp(Sbuffer,"BYE")==0)
						break;
				}
			}
		}
		else	// impossible to chat - kill process
			kill(pid,SIGKILL);	
	}

	// kill and close
	kill(pid,SIGKILL);
	close(connectFD);		

	return 0;
}

