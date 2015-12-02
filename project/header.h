# include <stdio.h>
# include <string.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <sys/poll.h>
# include <unistd.h>
# define PORT 10001
# define MAXLINE 100
# define BUFFER_SIZE 1000
# include <fcntl.h>
# include <signal.h>
# include <wait.h>
# include <stdlib.h>
//readvn.cpp
# include <errno.h>

struct MSGA
{
	char id[BUFFER_SIZE];
	char msg[BUFFER_SIZE];
};

struct MSGB
{
	char id[BUFFER_SIZE];
	char msg[BUFFER_SIZE];
};

struct MSGC
{
	char id[BUFFER_SIZE];
	char msg[BUFFER_SIZE];
};

struct MSGA initA(struct MSGA T)
{
	sprintf(T.id,"%s","N"); 
	return T;
}

struct MSGB initB(struct MSGB T)
{
	sprintf(T.id,"%s","N"); 
	return T;
}

struct MSGC initC(struct MSGC T)
{
	sprintf(T.id,"%s","N"); 
	return T;
}

ssize_t readn(int socketFD, char* buffer, size_t length) {

	ssize_t readBytes = 0;
	size_t leftBytes = length; 
	char* pointer = buffer;
	
	while (leftBytes > 0) {
		readBytes = read(socketFD, pointer, length);
		if (readBytes < 0) {
			if (errno == EINTR) continue;
			else return -1;
		} else if (readBytes == 0) {
			break;
		}
		
		leftBytes -= readBytes; 
		pointer += readBytes;
	}
	return length - leftBytes;
}


ssize_t readvn(int socketFD, char* buffer, size_t maxLength) {

	ssize_t readBytes = 0;
	size_t valueLength;

	readBytes = readn(socketFD, (char*)&valueLength, sizeof(size_t));
	if (readBytes != sizeof(size_t)) {
		if(readBytes < 0) return -1;
		else return 0;
	}
	
	valueLength = ntohl(valueLength);
	if (valueLength > maxLength) {
		while (valueLength > 0) {
			readBytes = readn(socketFD, buffer, maxLength);
			if (readBytes != maxLength) {
			
				if(readBytes < 0) return -1;
				
				else return 0;
			}
			valueLength -= readBytes; 
			if (valueLength < maxLength) maxLength = valueLength;
		}
		
		errno = EMSGSIZE;
		
		return -1;
		
	}
	
	readBytes = readn(socketFD, buffer, valueLength);
	
	if (readBytes != valueLength) {
		if(readBytes < 0) return -1;
		else return 0;
	}
	
	return readBytes;
}




ssize_t readline(int fd, char *ptr, size_t maxlen)
{
	ssize_t n,rc;
	char c;
	for(n=1;n<maxlen; n++)
	{
		if((rc=read(fd, &c, 1))==1)
		{
			*ptr++ =c;
			
			if(c=='\n')
				break;
		}
		
		else if(rc==0)
		{
			if(n==1)
				return 0;
				
			else
				break;
		}
	}
	*ptr =0;
	return n;
}






ssize_t writen(int socketFD, const char* buffer, size_t length) {
	ssize_t writtenBytes = 0;
	size_t leftBytes = length;
	char* pointer = (char*)buffer;
	while (leftBytes > 0) {
	
		writtenBytes = write(socketFD, pointer, length);
		
		if (writtenBytes <= 0) {
		
			if (errno == EINTR) continue;
			
			else return -1;
			
		}
		
		leftBytes -= writtenBytes; 
		
		pointer += writtenBytes;
		
	}
	
	return length - leftBytes;
}


ssize_t writevn(int socketFD, const char* buffer, size_t length) {

	ssize_t writtenBytes = 0;
	
	size_t valueLength = htonl(length);
	
	writtenBytes = writen(socketFD, (char*) &valueLength, sizeof(size_t));
	
	if (writtenBytes != sizeof(size_t)) {
		if(writtenBytes < 0) return -1;
		else return 0;
	}
	writtenBytes = writen(socketFD, buffer, length); 
	if (writtenBytes != length) {
		if(writtenBytes < 0) return -1;
		else return 0;
	}
	return writtenBytes;
}



