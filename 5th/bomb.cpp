#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int beeps =0;

void handler(int signal)
{
	printf("BEEP\n");
	fflush(stdout);

	if(++beeps < 5)
		alarm(1);
	else
	{
		printf("BOOM\n");
		exit(0);
	}
}

int main()
{
	signal(SIGALRM, handler);
	alarm(1);
	
	while(1)
	{
	}
	return 0;
}
