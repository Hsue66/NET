#include <stdio.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 10001
#define BUFFER_SIZE 100


#include "errno.h"

ssize_t readn(int socketFD, char* buffer, size_t length)
{
	size_t nleft;
	ssize_t nread;
	char *ptr;
	
	ptr = buffer;
	nleft = length;
	
	while(nleft > 0 )
	{
		if((nread = read(socketFD,ptr,nleft)) <0)
		{
			if(errno == EINTR)
				nread = 0 ;
			else
				return -1;
		}
		else if(nread == 0)
			break;
		nleft -= nread;
		ptr+= nread;
	}
	return (length - nleft);
}

ssize_t readvn(int socketFD, char* buffer, size_t maxLength)
{
	int reclen;
	int rc;
	
	rc = readn(socketFD, (char*)&reclen, sizeof(int));
	if(rc != sizeof(int))
		return rc <0 ? -1 :0;
	
	reclen = ntohl(reclen);
	
	if(reclen>maxLength)
	{
		while(reclen > 0 )
		{
			rc = readn(socketFD, buffer, maxLength);
			if(rc != maxLength)
				return rc <0?-1:0;
			reclen -= maxLength;
			if(reclen < maxLength)
				maxLength = reclen;
		}
		errno=EMSGSIZE;
		return -1;
	}
	rc = readn(socketFD, buffer, reclen);
	if(rc != reclen)
		return rc <0?-1:0;
	return rc;
}


ssize_t writen(int socketFD, const char* buffer, size_t length)
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;
	
	ptr = buffer;
	nleft = length;
	while(nleft > 0)
	{
		if((nwritten = write(socketFD, buffer, nleft)) <= 0)
		{
			if(nwritten < 0 && errno ==EINTR)
				nwritten =0;
			else
				return -1;
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	return length;
}


ssize_t writevn(int socketFD, const char* buffer, size_t length)
{
	int reclen;
	int rc;
	
	rc = writen(socketFD, (char*)&reclen, sizeof(int));
	if(rc != sizeof(int))
		return rc <0 ? -1:0;
	
	reclen = ntohl(reclen);
	if(reclen>length)
	{
		while(reclen > 0)
		{
			rc = writen(socketFD,buffer, length);
			if(rc != length)
				return rc <0?-1:0;
			reclen -= length;
			if(reclen <length)
				length = reclen;
		}
		errno =EMSGSIZE;
		return -1;
	}
	rc = writen(socketFD, buffer, reclen);
	if(rc != reclen)
		return rc <0?-1:0;
	return rc;

}
