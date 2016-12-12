#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define BUF_SIZE 20

struct socket_st{
	pid_t pid;
	char text[BUF_SIZE];
};

int main(){
	int sockfd;
	int len;
	struct sockaddr_un address;
	int result;
	struct socket_st sock;
	char *name = "김형근";

	while(1){
		sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
		address.sun_family = AF_UNIX;
		strcpy(address.sun_path, "server_socket");
	
		len = sizeof(address);
		result = connect(sockfd, (struct sockaddr *)&address, len);
		if(result == -1){
			fprintf(stderr, "opps:consumer\n");
			return -1;
		}

		read(sockfd, &sock, sizeof(sock));
		if(!strcmp(sock.text, "exit")){
			close(sockfd);
			break;
		}
		printf("producer id: %d\n", sock.pid);
		printf("student_id: %s\n", sock.text);

		sock.pid = getpid();
		memset(sock.text, 0, BUF_SIZE);
		strcpy(sock.text, name);
		write(sockfd, &sock, sizeof(sock));

		close(sockfd);
	}
	return 0;
}
