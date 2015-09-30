#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>


# define N 4

int count =0;

void child_handler(int signal)
{
	int child_status;
	pid_t pid;

	while((pid = waitpid(-1, &child_status, WNOHANG)) > 0)
	{
		count--;
		printf("Received signal  %d from process %d\n", signal, pid);
	}
}


int main()
{
	signal(SIGCHLD, child_handler);

	pid_t pid[N];
	int child_status;
	count = N;

	for (int i =0; i<N; i++)
	{		
		if((pid[i] =fork())==0)
			exit(0);
	}

	while(count > 0)
	{;}

	return 0;
}
