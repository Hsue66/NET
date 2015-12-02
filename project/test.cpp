#include <stdio.h>
#include <time.h>

struct topicA
{
	char id[5];
	char msg[5];
}A;

struct topicA initA(struct topicA T)
{
	char temp[20];
	//temp="N";
	sprintf(T.id, "%s","N");
	return T;
}

int main()
{
	//time_t now;
	//struct tm *t;
	char buf[80];

	time_t now = time(NULL);
	struct tm *t = localtime(&now);

	sprintf(buf,"%04d-%02d-%-2d %02d:%02d:%02d"
			,t->tm_year +1900, t->tm_mon+1, t->tm_mday,t->tm_hour, t->tm_min, t->tm_sec);

//	printf("%s\n",buf);
	//char a[2]="a";
	//sprintf(A.msg,"%s",a);
	printf("%s\n",A.id);
	A = initA(A);
	printf("%s\n",A.id);
}
