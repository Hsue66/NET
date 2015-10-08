#include <stdio.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <wait.h>

#define PORT 10001
#define BUFFER_SIZE 100

void err_sys(const char* x)
{
	perror(x);
	exit(1);
}
