# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <semaphore.h>
# include <pthread.h>

void unix_error(const char *msg)
{    
	int errnum = errno;	    
	fprintf(stderr, "%s (%d: %s)\n", msg, errnum, strerror(errnum));	
	exit(EXIT_FAILURE);
}
