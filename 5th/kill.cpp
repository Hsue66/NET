#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>


# define N 4

int main()
{
	pid_t pid[N];
	int child_status;

	for (int i =0; i<N; i++)
	{		
		if((pid[i] =fork())==0)
			while(true);
	}

	for(int i =0; i<N; i++)
	{
		printf("Killing process %d\n",pid[i]);
		kill(pid[i],SIGINT);
	}

	for(int i =0; i<N; i++)
	{
		pid_t wpid = wait(&child_status);
		if(WIFEXITED(child_status))
			printf("Child %d terminated %d\n",wpid, WEXITSTATUS(child_status));
		else
			printf("Child %d terminated abnormally\n",wpid);

	}
	return 0;
}
