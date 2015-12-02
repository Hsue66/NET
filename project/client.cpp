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
	struct MSGA msgA;
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

		int temp[3]={0};	
		//while(1)
		//{
			sprintf(msgA.id,"%s","A");
			sprintf(msgA.msg, "%s", "hello");
			sprintf(msgA.ext,"%s", "helloahse");	
			writevn(connFD, (char *)&msgA, sizeof(msgA));
		//}
	}
	fflush(stdout);
	return 0;
}
