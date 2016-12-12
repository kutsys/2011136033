#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define RD_PATH "./pWrite"
#define WR_PATH "./cWrite"
#define BUF_SIZE 20

pid_t split(char *buffer[]){
	int i, c=0;
	char temp[BUF_SIZE];

	for(i=0; i<BUF_SIZE; i++){
		c++;
		if((*buffer)[i] == ' ')
				break;
	}
	memset(temp, 0, BUF_SIZE);
	strncpy(temp, *buffer, sizeof(char)*c);
	strncpy(*buffer, *(buffer+c), sizeof(char)*(BUF_SIZE-c));
	return (pid_t)atoi(temp);
}

int main(){
	int pipe_fd;
	int res;
	pid_t pid;
	char buffer[BUF_SIZE];
	char *name = "김형근";
	if(access(WR_PATH, F_OK) == -1){
		res = mkfifo(WR_PATH, 0777);
		if(res){
			fprintf(stderr, "Could not create fifo %s\n", WR_PATH);
			return -1;
		}
	}

	while(1){
		pipe_fd = open(WR_PATH, O_WRONLY);
		if(pipe_fd == -1){
			fprintf(stderr, "Could not open O_WRONLY fifo %s\n", WR_PATH);
			return -1;
		}
		printf("Enter start for starting(or exit for end): ");
		memset(buffer, 0, BUF_SIZE);

		sprintf(buffer, "%d %s", getpid(), name);
		res = write(pipe_fd, buffer, BUF_SIZE);
		if(res == -1){
			fprintf(stderr, "Write error on pipe\n");
			return -1;
		}

		close(pipe_fd);
		
		pipe_fd = open(RD_PATH, O_RDONLY);
		if(pipe_fd == -1){
			fprintf(stderr, "Could not open O_RDONLY fifo %s\n", RD_PATH);
			return -1;
		}
		memset(buffer, 0, BUF_SIZE);
		res = read(pipe_fd, buffer, BUF_SIZE);
		pid = split(&buffer);
		printf("producer pid: %d\n", pid);
		printf("student id: %s\n", buffer);

		close(pipe_fd);
	}
	return 0;
}
