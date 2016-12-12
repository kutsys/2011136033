#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define RD_PATH "./pWrite"
#define WR_PATH "./cWrite"
#define BUF_SIZE 100

int main(){
	int pipe_id[2];
	int res;
	pid_t pid;
	char *buffer = (char*)malloc(sizeof(char)*BUF_SIZE);
	char *name = "김형근\0";
	if(access(WR_PATH, F_OK) == -1){
		res = mkfifo(WR_PATH, 0777);
		if(res){
			fprintf(stderr, "Could not create fifo %s\n", WR_PATH);
			return -1;
		}
	}

	pipe_id[0] = open(RD_PATH, O_RDONLY);
	pipe_id[1] = open(WR_PATH, O_WRONLY);

	while(1){
		memset(buffer, 0, BUF_SIZE);
		res = read(pipe_id[0], buffer, BUF_SIZE);
		if(!strcmp(buffer, "exit"))
				break;
		printf("%s\n", buffer);

		memset(buffer, 0, BUF_SIZE);
		sprintf(buffer, "consumer pid: %d, name: %s",getpid(), name);
		res = write(pipe_id[1], buffer, BUF_SIZE);
		if(res == -1){
			fprintf(stderr, "Write error on pipe\n");
			return -1;
		}
	}

	close(pipe_id[0]);
	close(pipe_id[1]);
	return 0;
}
