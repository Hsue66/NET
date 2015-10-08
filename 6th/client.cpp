#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>

#define PORT 10001
#define BUFFER_SIZE 5


int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Usage: %s IPv4-address\n", argv[0]);
		return -1;
	}
	struct sockaddr_in connectSocket;
	memset(&connectSocket, 0, sizeof(connectSocket));
	connectSocket.sin_family = AF_INET;
	inet_aton(argv[1], (struct in_addr*) &connectSocket.sin_addr.s_addr);
	connectSocket.sin_port = htons(PORT);
	int connectFD = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(connectFD, (struct sockaddr*) &connectSocket, sizeof(connectSocket)) == -1) {
		printf("Can not connect.\n");
		return -1;
	} else {
		int readBytes, writtenBytes;
		char sendBuffer[BUFFER_SIZE];
		char receiveBuffer[BUFFER_SIZE];
		while (true) {
			//서버에 문자열을 보낸 뒤 서버가 보낸 echo를 받아 출력하시오.
			 scanf("%s",sendBuffer);
			 write(connectFD,sendBuffer, BUFFER_SIZE);

			 readBytes = read(connectFD,receiveBuffer, BUFFER_SIZE);
			 receiveBuffer[readBytes]='\0';
			 fputs(receiveBuffer, stdout);
			 fflush(stdout);
		}
	}
	close(connectFD);
	return 0;
}
