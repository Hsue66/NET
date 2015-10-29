//client.cpp
# include "header.h"
# include <pthread.h>
# include <time.h>
//pthread를 위한 헤더를 추가하시오

void randomMessage(int id, int connectFD)
{
	//writevn, readvn으로 두 랜덤 넘버를 전송하고 그 합을 받으시오.

	char receiveBuffer[BUFFER_SIZE];
    char sendBuffer[BUFFER_SIZE] ;

	srand(time(NULL));
    int	random = 100+rand()%100;
	
	sprintf(sendBuffer ,"%d",random);
	writevn(connectFD, sendBuffer, strlen(sendBuffer));


    random = 100+rand()%100;
	sprintf(sendBuffer ,"%d",random);
	writevn(connectFD, sendBuffer, strlen(sendBuffer));

    int n = readvn(connectFD,receiveBuffer,BUFFER_SIZE);	
	printf("%d : %s\n",id,receiveBuffer);

	fflush(stdout);         
	
}

int main(int argc, char** argv)
{
	//순차적으로 ID를 생성하면서 10개의. 클라이언트 프로세스를 fork하시오.
	//자식 프로세스일 경우 서버와 접속하여 통신을 10번 반복한 후 접속을 끊으시오
	//부모 프로세스일 경우 10개의 자식 프로세스가 끝나기를 기다리시오

	if (argc != 2) 
	{
		printf("Usage: %s IPv4-address\n", argv[0]);
		return -1;
	}
	

	pid_t pid[10];
	int count=0;
	while(count<10)
	{
		if((pid[count]=fork())==0)
		{
	
			struct sockaddr_in connectSocket;
			memset(&connectSocket, 0,sizeof(connectSocket));
			connectSocket.sin_family = AF_INET;
			inet_aton(argv[1], (struct in_addr*) &connectSocket.sin_addr.s_addr);
			connectSocket.sin_port = htons(PORT);

			int connectFD = socket(AF_INET, SOCK_STREAM, 0);

			if(connect(connectFD, (struct sockaddr*) &connectSocket, sizeof(connectSocket)) == -1)
			{
				printf("Cannot connect.\n");
				return -1;
			}
			else
			{
				randomMessage(count+1, connectFD);
				
			}
			close(connectFD);
			exit(0);
		}
		else
		{
			pid_t wpid = waitpid(pid[count],NULL,0);
		}
		count++;	
	}
	return 0;
}	
