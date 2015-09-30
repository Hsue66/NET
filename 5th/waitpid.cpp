#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <wait.h>

#define N 4

int main()
{
	pid_t pid[N];
	int child_status;

	for (int i =0 ; i<N; i++)
	{
		if((pid[i] = fork())==0)
			exit(100+i);
	}

	for(int i=0; i<N; i++)
	{
		pid_t wpid = waitpid(pid[i],&child_status, 0);
		if(WIFEXITED(child_status))
			printf("Child %d terminates %d\n",wpid, WEXITSTATUS(child_status));
		else
			printf("Child %d terminated abnormally \n",wpid);
		
	}
	return 0;
}

