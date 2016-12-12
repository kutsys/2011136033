#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 20

struct socket_st{
	pid_t pid;
	char text[BUF_SIZE];
};

int main(){
	int producer_sockfd, consumer_sockfd;
	int producer_len, consumer_len;
	struct sockaddr_un producer_address;
	struct sockaddr_un consumer_address;
	char *student_id = "2011136033", buffer[BUF_SIZE];

	unlink("server_socket");
	producer_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

	producer_address.sun_family = AF_UNIX;
	strcpy(producer_address.sun_path, "server_socket");
	producer_len = sizeof(producer_address);
	bind(producer_sockfd, (struct sockaddr *)&producer_address, producer_len);

	listen(producer_sockfd, 5);
	while(1){
		struct socket_st sock;
		
		consumer_len = sizeof(consumer_address);
		consumer_sockfd = accept(producer_sockfd, (struct sockaddr*)&consumer_address, &consumer_len);

		printf("Enter start for starting(or exit for end): ");
		memset(buffer, 0, BUF_SIZE);
		fgets(buffer, BUF_SIZE, stdin);

		sock.pid = getpid();
		memset(sock.text, 0, BUF_SIZE);
		strcpy(sock.text, student_id);

		if(!strcmp(buffer, "exit\n")){
			memset(sock.text, 0, BUF_SIZE);
			strncpy(sock.text, "exit", BUF_SIZE);
			write(consumer_sockfd, &sock, sizeof(sock));
			close(consumer_sockfd);
			break;
		}
		if(strcmp(buffer,"start\n")){
			close(consumer_sockfd);
			continue;
		}

		write(consumer_sockfd, &sock, sizeof(sock));
		read(consumer_sockfd, &sock, sizeof(sock));
		printf("producer pid: %d\n", getpid());
		printf("consumer pid: %d\n", sock.pid);
		printf("student_id: %s\n", student_id);
		printf("name: %s\n", sock.text);
		close(consumer_sockfd);
	}

	close(producer_sockfd);
	return 0;
}
