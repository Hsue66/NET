#include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <errno.h>
# include <sys/types.h>
# include <wait.h>
#include <semaphore.h>
#include <pthread.h>
void unix_error(const char *msg)
{
	    int errnum = errno;
		    fprintf(stderr, "%s (%d: %s)\n", msg, errnum, strerror(errnum));
			    exit(EXIT_FAILURE);
}
